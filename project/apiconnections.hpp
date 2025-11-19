#ifndef APICONNECTIONS
#define APICONNECTIONS

#include <ArduinoJson.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "dateparsing.hpp"
#include "simple_error_struct.hpp"

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LilyGo_AMOLED.h>

const std::string STATION_ID = "65090";
const std::string LONGITUDE = "15.586710";
const std::string LATITUDE = "56.160820";

std::string makeRequest(const std::string& url, Error& err) {
    HTTPClient http;
    if (!http.begin(url.c_str())) {
        err.msg = "Failed to initialize HTTP client for URL: " + url;
        return "";
    }

    int responseCode = http.GET();
    if (responseCode <= 0) {
        err.msg = "HTTP error: " + std::to_string(responseCode) +
                  " (" + http.errorToString(responseCode).c_str() + ")";
        return "";
    }

    WiFiClient* stream = http.getStreamPtr();
    std::string payload;

	if ((char)stream->peek() == '{') {
		const size_t bufferSize = 512;
		uint8_t buffer[bufferSize];

		while (true) {
    		size_t len = stream->readBytes(buffer, bufferSize);
    		if (len == 0) break;     // no more data
    		payload.append((char*)buffer, len);
		}
	} else while (http.connected()) {
        // 1. Read chunk length line
        String lenStr = stream->readStringUntil('\n');
        lenStr.trim();
        if (lenStr.length() == 0) continue;

        // chunk length is hex
        int chunkSize = (int) strtol(lenStr.c_str(), NULL, 16);
        if (chunkSize == 0) {
            break;  // done
        }

        // 2. Read the exact number of bytes in the chunk
        payload.reserve(payload.size() + chunkSize);
        while (chunkSize > 0) {
            if (!stream->available()) {
                delay(1);
                continue;
            }

            uint8_t buffer[256];
            int n = stream->read(buffer, min(chunkSize, (int)sizeof(buffer)));
            if (n > 0) {
                payload.append((char*)buffer, n);
                chunkSize -= n;
            }
        }

        // 3. Read the trailing CRLF
        stream->read(); // '\r'
        stream->read(); // '\n'
    }

    return payload;
}

void dezerializeJson(JsonDocument& doc, const std::string& payload, Error& err) {
	DeserializationError error = deserializeJson(doc, payload);
	if (error) {
		err.msg = "deserializeJson() failed: " + std::string(error.c_str()) + "\n" + payload.substr(0,32);
	}
}

std::vector<std::pair<std::string, std::string>> getAllStations(Error& err) {
	std::string payload =
	    makeRequest("https://opendata-download-metobs.smhi.se/api/version/latest/parameter/1/station.json", err);
	JsonDocument doc;
	dezerializeJson(doc, payload, err);
	if (err) {
		return {};
	}

	JsonArray stations = doc["station"];
	std::vector<std::pair<std::string, std::string>> out = {};
	for (size_t i = 0; i < stations.size(); ++i) {
		std::pair<std::string, std::string> station;
		JsonString stationName = stations[i]["title"];
		JsonString stationKey = stations[i]["key"];
		station.first = std::string(stationName.c_str());
		station.second = std::string(stationKey.c_str());
		out.push_back(station);
	}
	return out;
}

struct HistoricData {
	Date date;
	float data;
	std::string unit;
};

enum HistoricDataParameters { HISTORIC_TEMP = 1, HISTORIC_RAIN_AMOUNT = 5, HISTORIC_WIND_SPEED = 4, HISTORIC_HUMIDITY = 6 };

std::vector<HistoricData> getHistoricDataFromId(std::string id, int parameter, Error& err) {
	std::string payload =
	    makeRequest("https://opendata-download-metobs.smhi.se/api/version/latest/parameter/" +
	                    std::to_string(parameter) + "/station/" + id + "/period/latest-months/data.json",
	                err);
	if (err) {
		return {};
	}
	
	JsonDocument doc;
	#ifdef DEBUG
	Serial.println((String)payload.c_str());
	#endif
	dezerializeJson(doc, payload, err);
	if (err) {
		return {};
	}

	const char* parameterUnit = doc["parameter"]["unit"];

	std::vector<HistoricData> out = {};

	JsonArray values = doc["value"];

	size_t valuesAmount = values.size();

	for (size_t i = 0; i < valuesAmount; i++) {
		if (valuesAmount > 200 && i == 0) i = 11;
		HistoricData data;
		data.data = values[i]["value"].as<float>();
		data.unit = parameterUnit;
		uint64_t unixTime = values[i]["date"].as<uint64_t>();
		if (unixTime) {
			data.date = unixToDate(unixTime);
		} else {
			data.date = ShortDateParser(values[i]["ref"], err);
		}
		out.push_back(data);
		if (valuesAmount > 200) i += 23;
	}
	
	return out;
}

std::vector<HistoricData> getHistoricDataFromId(int id, int parameter, Error& err) {
	return getHistoricDataFromId(std::to_string(id), parameter, err);
}


struct ForecastTemp {
	Date date;
	float temp;
	const char* tempUnit = "Cel";

	float windSpeed;
	const char* windSpeedUnit = "m/s";
	float windDirection;
	const char* windDirectionUnit = "degree";

	float precipitationMean;
	const char* precipitationMeanUnit = "kg/m^2";

	float rainProbability;
	const char* rainProbabilityUnit = "%";

	float cloudAreaFraction;
	const char* cloudAreaFractionUnit = "octas";

	float visibility;
	const char* visibilityUnit = "km";

	float thunderProbability;
	const char* thunderProbabilityUnit = "%";

	float snowProbability;
	const char* snowProbabilityUnit = "fraction";

	std::string getAllData() {
		std::string out;
        out.reserve(512);              // PRE-ALLOCATE for speed

        char num[32];                  // stack buffer for float → string

        out += date.ymdhms();
        out += ":\n";

        snprintf(num, sizeof(num), "%.2f", temp);
        out += "\ttemperature: "; out += num; out += " "; out += tempUnit; out += "\n";

        snprintf(num, sizeof(num), "%.2f", windSpeed);
        out += "\twindspeed: "; out += num; out += " "; out += windSpeedUnit; out += "\n";

        snprintf(num, sizeof(num), "%.1f", windDirection);
        out += "\twind direction: "; out += num; out += " "; out += windDirectionUnit; out += "\n";

        snprintf(num, sizeof(num), "%.2f", precipitationMean);
        out += "\tprecipitation mean: "; out += num; out += " "; out += precipitationMeanUnit; out += "\n";

        snprintf(num, sizeof(num), "%.1f", rainProbability);
        out += "\train probability: "; out += num; out += rainProbabilityUnit; out += "\n";

        snprintf(num, sizeof(num), "%.2f", cloudAreaFraction);
        out += "\tcloud area fraction: "; out += num; out += " "; out += cloudAreaFractionUnit; out += "\n";

        snprintf(num, sizeof(num), "%.1f", visibility);
        out += "\tvisibility: "; out += num; out += " "; out += visibilityUnit; out += "\n";

        snprintf(num, sizeof(num), "%.1f", thunderProbability);
        out += "\tthunder probability: "; out += num; out += thunderProbabilityUnit; out += "\n";

        snprintf(num, sizeof(num), "%.2f", snowProbability);
        out += "\tsnow probability: "; out += num; out += " "; out += snowProbabilityUnit; out += "\n";

        return out;
	}
};

std::vector<ForecastTemp> getForecastFromLongAndLat(std::string longitude, std::string latitude, Error& err) {
	std::string url = "https://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/geotype/point/lon/"+longitude+"/lat/"+latitude+"/data.json";
	std::string payload = makeRequest(url, err);
	if (err) {
		return {};
	}
	
	JsonDocument doc;
	#ifdef DEBUG
	Serial.println((String) payload.c_str());
	#endif
	dezerializeJson(doc, payload, err);
	if (err) {
		return {};
	}

	std::vector<ForecastTemp> out;

	JsonArray dates = doc["timeSeries"];

	size_t datesAmount = dates.size();

	

	for (size_t i = 0; i < datesAmount; i++) {
		ForecastTemp temp;
		JsonObject data = dates[i]["data"];
		temp.date = ISO8601DateParser(dates[i]["time"], err);
		if (err) {
			return {};
		}
		if (!(i == 0 || temp.date.hour == 12)) continue;
		temp.windDirection = data["wind_from_direction"].as<float>();
		temp.windSpeed = data["wind_speed"].as<float>();
		temp.temp = data["air_temperature"].as<float>();
		temp.precipitationMean = data["precipitation_amount_mean"].as<float>();
		temp.rainProbability = data["probability_of_precipitation"].as<float>();
		temp.cloudAreaFraction = data["cloud_area_fraction"].as<float>();
		temp.visibility = data["visibility_in_air"].as<float>();
		temp.thunderProbability = data["thunderstorm_probability"].as<float>();
		temp.snowProbability = data["probability_of_frozen_precipitation"].as<float>();

		out.push_back(temp);
	}
	return out;
}

std::vector<ForecastTemp> getForecastFromLongAndLat(float longitude, float latitude, Error& err) {
	return getForecastFromLongAndLat(std::to_string(longitude), std::to_string(latitude), err);
}

#endif