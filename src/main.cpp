// main.cpp

#include <iostream>
#include <sstream>      // std::stringstream
#include <string>  // for std::to_string

#include <ppltasks.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <array>

#include "Item.h"


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency;				// Asynchronous
using namespace concurrency::streams;       // Asynchronous streams

typedef std::wstring			String;
typedef std::wstringstream		StringStream;

/// @brief:		serialize a given json::value into a c++ string
/// @arg0:		json_value		the json_value which will get converted to a string in c++
/// @return:	the c++ string to the passed json::value
/// @note:		similar to json::value::serialize(ostream), though you have no control on what to do with it
String get_json_as_string(const json::value &json_value) {
	StringStream ss;

	if (json_value.is_array()) {
		json::array array = json_value.as_array();
		ss << "[ ";
		std::for_each(array.begin(), array.end(), [&](web::json::value jv)
		{
			ss << jv.as_string() << "   ";
		});
		ss << " ]";
	}
	else if (json_value.is_boolean()) {
		ss << json_value.as_bool();
	}
	else if (json_value.is_double()) {
		ss << json_value.as_double();
	}
	else if (json_value.is_integer()) {
		ss << json_value.as_integer();
	}
	else if (json_value.is_null()) {
		ss << "NULL";
	}
	else if (json_value.is_number()) {
		ss << json_value.as_number().to_uint64();
	}
	else if (json_value.is_object()) {
		auto dataObj = json_value.as_object();	// "An object is an unordered set of name/value pairs" web::json::object
		ss << "\n{\n";
		for (auto iterInner = dataObj.cbegin(); iterInner != dataObj.cend(); ++iterInner)
		{
			auto &propertyName = iterInner->first;		// utility::string_t
			auto &propertyValue = iterInner->second;	// web::json::value

			ss << "    \"" << propertyName << "\": \"" << get_json_as_string(propertyValue) << "\", \n";
		}
		ss << "}"; 
	}
	else if (json_value.is_string()) {
		ss << json_value.as_string().c_str();
	}
	else {
		ss << "UNDEFINED";
	}

	return ss.str();
} 

/// @brief:		send an http GET request to the given uri and extract the json from the http body
/// @arg0:		uri u		the uri the http GET request is being sent to
/// @return:	the ppl::task of template type web::json::value
pplx::task<web::json::value> json_request_task(uri u) {
	// requestTask provides us a placeholder for a value that will be available in the future.
	//pplx::task<web::json::value> requestTask =		// create_task also possible here
	return create_task( [=]() ->  pplx::task<web::http::http_response>		// [=] capture all locals by copy into lambda
			{
				// Build request URI and start the request.
				// uri_builder builder(u);
				std::wcout << u.to_string() << "\n";

				// Create http_client to send the request.
				web::http::client::http_client client(u);
				return client.request(web::http::methods::GET);
			}
		)
		// Handle response headers arriving. (task continuation when http header arrives)
		// "then" attaches a handler function to the task, which will be called asynchronolsy when the task completes
		// In asynchronous programming, it's common to define a sequence of operations, also known as task chains, in which each continuation executes only when the previous one completes. 
		// return type of the lambda function is wrapped in a task object
		.then( [=](pplx::task<web::http::http_response> previousTask) -> pplx::task<web::json::value>	// task-based continuation here!
			{
				// Since we know the task has finished, calling ‘get()’ inside the callback function object is never going to block. (task.get)
				const web::http::http_response&  response = previousTask.get();

				std::wcout << "Received response status code:" << response.status_code() << std::endl;

				// if the status is OK extract the body of the response into a JSON value
				// works only when the content type is application\json
				if (response.status_code() == status_codes::OK)
				{
					std::wcout << "STATUS_CODE:" << status_codes::OK << " OK \n";
					return response.extract_json();	// extract the json body from the http response and forward it
				}
				// otherwise return an empty JSON value
				return pplx::task_from_result(web::json::value());
			}
		)
	;
}

#include <QApplication>
#include "QJsonWidget.h"

#include <qstringlist.h>
#include <qabstractitemmodel.h>
#include <qstringlistmodel.h>

/// main -- test of gw2 rest api -- kinda similar to a web-crawler
int main(int argc, char* argv[])
{
	auto shared_model = QSharedPointer<QStringListModel>::create();	// you cannot simply add a row to it

	QApplication app(argc, argv);
	QJsonWidget *jwidget = new QJsonWidget(0,0,shared_model.get());
	jwidget->show(); 

	std::cout << "---------------------------" << std::endl;
	std::cout << "hello cpp rest sdk :D" << std::endl;

	// auto fileStream = std::make_shared<ostream>();	// so its accessible concurrently to the request-task and main-task
	// auto jar = std::make_shared<web::json::value>();

	// Build request URI and start the request.
	uri_builder builder(U("https://api.guildwars2.com/v2/items"));

	// retrieve the item-list from gw2 rest api
	auto requested_task = json_request_task(builder.to_uri());

	requested_task
		.then([=](pplx::task<web::json::value> previousTask)	// capture all local vars into lambda
	{
		// get the JSON value from the task and display content from it
		const web::json::value& json_value = previousTask.get();	// web::json::value     // Wait for the tasks to finish 

		//json_value.serialize(std::wcout);

		const web::json::array ar = json_value.as_array();

		// iterate over each items properties from gw2 rest api
		std::for_each(
			ar.cbegin(),
			ar.cend(),
			[&](web::json::value jv)
		{
			const int id = jv.as_integer();
			// std::cout << "val : " << id << "\n";
			uri_builder builder_subEndpoint(uri(builder.to_string() + U("/") + std::to_wstring(id)));

			pplx::task<web::json::value> requested_inner_task = json_request_task(builder_subEndpoint.to_uri());

			const web::json::value json_value2 = requested_inner_task.get();
			// std::wcout << get_json_as_string(json_value2) << "\n";
			// json_value2.serialize(std::wcout);


			// first add a row to the model, and then change it's data to the givenString
			const std::string givenSandardString = std::to_string(id);
			QString givenString = QString::fromStdString(givenSandardString);
			std::cout << "givenString: " << givenString.toStdString() << std::endl;
			if (shared_model->insertRow(shared_model->rowCount())) {
				QModelIndex index = shared_model->index(shared_model->rowCount() - 1, 0);
				shared_model->setData(index, givenString);
			} 
		}
		);
	})
		/////////////////////////////////
		// exception handling here:
		.then([=]()
	{
		try
		{
			// exceptions from ascending tasks are passed down here for error handling
		}
		catch (json::json_exception &je)
		{
			std::cerr << "JSON exception occurred:" << je.what() << "\n";
		}
		catch (pplx::task_canceled &tc)
		{
			std::cerr << "Task canceled exception occurred:" << tc.what() << "\n";
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error exception occurred:" << e.what() << "\n";
		}
	});
	std::cout << "\n\nend" << std::endl;
	std::cout << "---------------------------" << std::endl;
	// return 0;


	return app.exec();

}