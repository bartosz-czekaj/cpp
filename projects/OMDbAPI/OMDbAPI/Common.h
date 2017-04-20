#pragma once
#ifdef OMDBAPI_EXPORTS  
#define CLASS_EXPORT __declspec(dllexport)   
#else  
#define CLASS_EXPORT __declspec(dllimport)   
#endif 

CLASS_EXPORT enum class searchedType
{
	any,
	movie,
	series,
	episode
};

CLASS_EXPORT enum class responseType
{
	xml,
	json
};

CLASS_EXPORT enum class plotType
{
	getFull,
	getShort
};

struct CLASS_EXPORT commonElements
{
	unsigned int  year = 0;
	responseType response = responseType::json;
	searchedType type = searchedType::any;
	plotType plot = plotType::getShort;

};