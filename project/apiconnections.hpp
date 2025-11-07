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

const std::string LONGITUDE = "15.586710";
const std::string LATITUDE = "56.160820";

std::string makeRequest(std::string url, Error& err) {
	HTTPClient http;
	http.begin(url.c_str());
	int responseCode = http.GET();
    
	if (responseCode > 0) {
		std::string payload = http.getString().c_str();
		return payload;
	}
	err.msg = "Error: could not make request";
	return "";
}

void dezerializeJson(JsonDocument& doc, const std::string& payload, Error& err) {
	DeserializationError error = deserializeJson(doc, payload);
	if (error) {
		err.msg = "deserializeJson() failed: " + std::string(error.c_str()) + "\n";
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


struct HistoricTemp {
	Date date;
	std::string temp;
	std::string unit;
};

std::vector<HistoricTemp> getHistoricTempFromId(std::string id, Error& err) {
	std::string payload =
	    makeRequest("https://opendata-download-metobs.smhi.se/api/version/latest/parameter/1/station/" + id +
	                    "/period/latest-months/data.json",
	                err);
	if (err) {
		return {};
	}

	JsonDocument doc;
	dezerializeJson(doc, payload, err);
	if (err) {
		return {};
	}

	const char* parameterUnit = doc["parameter"]["unit"];

	std::vector<HistoricTemp> out = {};

	JsonArray values = doc["value"];
	for (size_t i = 0; i < values.size(); i++) {
		HistoricTemp temp;
		temp.temp = values[i]["value"].as<std::string>();
		temp.unit = parameterUnit;
		time_t unixTime = values[i]["date"];
		temp.date = unixToDate(unixTime);
		if (temp.date.hour == 12) out.push_back(temp);
	}

	return out;
}

std::vector<HistoricTemp> getHistoricTempFromId(int id, Error& err) {
	return getHistoricTempFromId(std::to_string(id), err);
}

struct ForecastTemp {
	Date date;
	std::string temp;
	std::string tempUnit = "Cel";

	std::string windSpeed;
	std::string windSpeedUnit = "m/s";
	std::string windDirection;
	std::string windDirecitonUnit = "degree";

	std::string precipitationMean;
	std::string precipitationMeanUnit = "kg/m^2";

	std::string rainProbability;
	std::string rainProbabilityUnit = "%";

	std::string cloudAreaFraction;
	std::string cloudAreaFractionUnit = "octas";

	std::string visibility;
	std::string visibilityUnit = "km";

	std::string thunderProbability;
	std::string thunderProbabilityUnit = "%";

	std::string snowProbability;
	std::string snowProbabilityUnit = "fraction";

	std::string getAllData() {
		return date.ymdhms() + " : " + "\n\ttemperature: " + temp + " " + tempUnit +
		       "\n\twindspeed and dir: " + windSpeed + " " + windSpeedUnit + ", " + windDirection + " " +
		       windDirecitonUnit + "\n\tprecipitation mean (rain): " + precipitationMean + " " + precipitationMeanUnit +
		       "\n\train prob: " + rainProbability + " " + rainProbabilityUnit +
		       "\n\tcloud area fraction: " + cloudAreaFraction + " " + cloudAreaFractionUnit +
		       "\n\tvisibility: " + visibility + " " + visibilityUnit +
		       "\n\tthunder probability: " + thunderProbability + " " + thunderProbabilityUnit +
		       "\n\tsnow probability: " + snowProbability + " " + snowProbabilityUnit + "\n";
	}
};

std::vector<ForecastTemp> getForecastFromLongAndLat(std::string longitude, std::string latitude, Error& err) {
	std::string payload =
	    makeRequest("https://opendata-download-metfcst.smhi.se/api/category/snow1g/version/1/geotype/point/lon/" +
	                    longitude + "/lat/" + latitude + "/data.json",
	                err);

	JsonDocument doc;
	dezerializeJson(doc, payload, err);
	if (err) {
		return {};
	}

	std::vector<ForecastTemp> out;

	JsonArray dates = doc["timeSeries"];

	for (size_t i = 0; i < dates.size(); i++) {
		ForecastTemp temp;
		JsonObject data = dates[i]["data"];
		temp.date = ISO8601DateParser(dates[i]["time"], err);
		if (err) {
			return {};
		}
		temp.windDirection = std::to_string((JsonFloat)data["wind_from_direction"]);
		temp.windSpeed = std::to_string((JsonFloat)data["wind_speed"]);
		temp.temp = std::to_string((JsonFloat)data["air_temperature"]);
		temp.precipitationMean = std::to_string((JsonFloat)data["precipitation_amount_mean"]);
		temp.rainProbability = std::to_string((JsonFloat)data["probability_of_precipitation"]);
		temp.cloudAreaFraction = std::to_string((JsonFloat)data["cloud_area_fraction"]);
		temp.visibility = std::to_string((JsonFloat)data["visibility_in_air"]);
		temp.thunderProbability = std::to_string((JsonFloat)data["thunderstorm_probability"]);
		temp.snowProbability = std::to_string((JsonFloat)data["probability_of_frozen_precipitation"]);

		if (i == 0 || temp.date.hour == 12) out.push_back(temp);
	}
	return out;
}

std::vector<ForecastTemp> getForecastFromLongAndLat(float longitude, float latitude, Error& err) {
	return getForecastFromLongAndLat(std::to_string(longitude), std::to_string(latitude), err);
}

#endif