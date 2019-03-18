// main.cpp
#include <sqlite3.h>
#include <iostream>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <boost/filesystem.hpp>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int main()
{
	std::cout << "hello cpp rest sdk :D" << std::endl;
	boost::filesystem::path full_path(boost::filesystem::current_path());
	std::cout << "Current path is : " << full_path << std::endl;

	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask =			// requestTask is the asynchronos task.. the response from the server (may arrive fully in the future)
		// first open a file stream (shared to make it available to all units)
		fstream::open_ostream(U("results.html"))

		.then([=](ostream outFile)
		{
			*fileStream = outFile;

			// Create http_client to send the request.
			http_client client(U("https://api.guildwars2.com"));

			// Build request URI and start the request.
			uri_builder builder(U("/v2/items"));
			//builder.append_query(U("q"), U("cpprestsdk github"));
			return client.request(methods::GET, builder.to_string());
		})

		// Handle response headers arriving.
		.then([=](http_response response)		// "then" attaches a handler function to the task, which will be called asynchronolsy when the task completes
		{
			// Since we know the task has finished, calling ‘get()’ inside the callback function object is never going to block. (task.get)
			printf("Received response status code:%u\n", response.status_code());

			// Write response body into the file.
			//auto json_body = response.extract_json();
			return response.body().read_to_end(fileStream->streambuf());
		})

		// Close the file stream, once the writing is done
		.then([=](size_t)
		{
			return fileStream->close();
		})

		// you can add a task-based continuation at the end of the chain and handle all errors there.
		.then([](pplx::task<void> requestTask)
		{

			// Wait for all the outstanding I/O to complete and handle any exceptions
			try
			{
				requestTask.get();	// get any exception procuded down the task chain
				// note:   Only catch the exceptions that you can handle.
				// If your app encounters an error that you can't recover from, it's better to let the app crash than to let it continue to run in an unknown state.
			}
			catch (pplx::task_canceled &tc)
			{
				printf("Task canceled exception:%s\n", tc.what());
			}
			catch (const std::exception &e)
			{
				printf("Error exception:%s\n", e.what());
			}

		})
	;


	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	std::cout << "end" << std::endl;
    return 0;
}