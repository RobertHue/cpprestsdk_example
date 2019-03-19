# cpprestsdk

example of cpprestsdk using the gw2 rest api 

## What this example mainly offers:
This example of cpprestsdk offers a function for sending an http GET request to the given uri 
and extract the json from the http body

this function has the following prototype:

'''
/// @brief:		send an http GET request to the given uri and extract the json from the http body
/// @arg0:		uri u		the uri the http GET request is being sent to
/// @return:	the ppl::task of template type web::json::value
pplx::task<web::json::value> json_request_task(uri u);
'''

you can retrieve the pplx::task from the json_request_task-function with the following code snippet:

'''
	// retrieve the item-list from gw2 rest api
	auto requested_task = json_request_task(builder.to_uri());
'''

after retrieving the pplx::task this way you can add continuation chains with the .then method, like following code snippet:

'''
.then([=](pplx::task<web::json::value> previousTask)	// capture all local vars into lambda
	{
		// get the JSON value from the task and display content from it
		const web::json::value& json_value = previousTask.get();	// web::json::value     // Wait for the tasks to finish 

		json_value.serialize(std::wcout);

		const web::json::array ar = json_value.as_array();

		// iterate over each items properties from gw2 rest api
		std::for_each(
			ar.cbegin(),
			ar.cend(),
			[&](web::json::value jv)
			{
				std::wout << jv.to_string() << std::endl;
			}
		);
	}
);
'''

also exception handling can be handled in another chain contination:

'''
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
	}
);
'''

## ToDO / Future

I hope to add more functionality for it to be crawling through the gw2 rest api and printing out the items on some GUI

## Information

## How to install the cpprestsdk with vcpkg:
https://github.com/Microsoft/cpprestsdk#getting-started

## How to use vcpkg with cmake-gui:
https://developerpaul123.github.io/c++/using-vcpkg-on-windows/

## How to use the cpprestsdk:
https://github.com/Microsoft/cpprestsdk/wiki/Getting-Started-Tutorial

## How to work with pplx_tasks:
https://github.com/Microsoft/cpprestsdk/wiki/Programming-with-Tasks
https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2012/dd492427(v=vs.110)

## How to read/use the gw2 rest api:
https://wiki-de.guildwars2.com/wiki/API