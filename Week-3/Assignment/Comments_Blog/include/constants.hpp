#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

//Using c as prefix for constants to indicate that they are constant strings

const std::string prefix = "var tumblr_api_read = ";
const std::string cPhotoUrl1280 = "photo-url-1280";
const std::string cPhotoUrl500 = "photo-url-500";
const std::string cTumbleLog = "tumblelog";
const std::string cTitle = "title";
const std::string cName = "name";
const std::string cDescription = "description";
const std::string cNotApplicable = "N/A";
const std::string cPosts = "posts";
const std::string cUnexpectedResponseFormat = "Unexpected response format.";
const std::string cBlogInformationNotFound = "Blog information not found.";
const std::string cNoImageAvailable = "No image available.";
const std::string cNoPostsFound = "No posts found in the specified range.";

const std::string cEnterBlogName = "Enter the Tumblr blog name: ";
const std::string cEnterPostRange = "Enter the post range (start-end): ";
const std::string cInvalidRangeFormat = "Invalid range format. Use start-end format.";
const std::string cInvalidRangeValue = "Invalid range values.";

const std::string cWebUrlProtocol = "https://";
const std::string cWebUrlDomainName = ".tumblr.com";
const std::string cWebUrlResourcePath = "/api/read/json";
const std::string cWebUrlTypeQueryParameter = "?type=photo";
const std::string cWebUrlStartingIndexQueryParameter = "&start=";
const std::string cWebUrlTotalPostsQueryParameter = "&num=";
const std::string cFailedToIntializeCURL = "Failed to initialize cURL.";
const std::string cCURLRequestFailed = "cURL request failed: ";

#endif