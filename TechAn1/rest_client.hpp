//#pragma once
//
//#include <boost/asio.hpp>
//#include <boost/asio/ssl.hpp>
//#include <vector>
//#include <string>
////#include <urdl/istream.hpp>
////#include <urdl/http.hpp>
/////*std::vector<*/std::string/*>*/ getData(std::string const& valcode, std::string const& date, bool json) 
////{
////	urdl::istream is("https://bank.gov.ua/NBUStatService/v1/statdirectory");
////
////	// Set request options: method, content type, content
////	//is.set_option(urdl::http::request_method("GET"));
////	//is.set_option(urdl::http::request_content_type("application/json"));
////	//is.set_option(urdl::http::request_content("{\"username\":\"user@name.co\", \"password\":\"test\"}"));
////
////	// open HTTP stream at URL
////	//is.open("https://bank.gov.ua/NBUStatService/v1/statdirectory");
////	std::string response;
////	while (!is.eof()) 
////	{
////		std::string temp;
////		std::getline(is, temp);
////		response.append(temp);
////	}
////	return response;
////}
//
///*std::vector<*/std::string/*>*/ getData_(std::string const& valcode, std::string const& date, bool json) 
//{
//	using boost::asio::ip::tcp;
//
//	tcp::iostream stream;
//	//stream.expires_from_now(boost::posix_time::seconds(60));
//	stream.connect("www.boost.org", "http");
//	stream << "GET /LICENSE_1_0.txt HTTP/1.0\r\n";
//	stream << "Host: www.boost.org\r\n";
//	stream << "Accept: */*\r\n";
//	stream << "Connection: close\r\n\r\n";
//	stream.flush();
//
//	std::string res;
//	while (!stream.eof())
//	{
//		std::string temp;
//		std::getline(stream, temp);
//		res.append(temp);
//	}
//	return res;
//	//std::cout << stream.rdbuf();
//}
//
///*std::vector<*/std::string/*>*/ getData(std::string const& valcode, std::string const& date, bool json)
//{
//	using boost::asio::ip::tcp;
//	using boost::asio::ssl::context;
//
//	boost::asio::io_service io_svc;
//
//	context ctx(context::method::sslv23_client);
//	boost::asio::ssl::stream<tcp::socket> ssock(io_svc, ctx);
//
//	tcp::resolver resolver(io_svc);
//	auto it = resolver.resolve({ "bank.gov.ua", "http" });
//
//	//ssock.lowest_layer().connect({ "bank.gov.ua", "https" });
//	boost::asio::connect(ssock.lowest_layer(), it);
//	ssock.handshake(boost::asio::ssl::stream_base::handshake_type::client);
//
////	tcp::resolver resolver(io_svc);
////	//tcp::resolver::query query("bank.gov.ua", "https");
////	tcp::resolver::query query("bank.gov.ua", "http");
////	//tcp::resolver::query query("bank.gov.ua", "https");
//////	tcp::resolver::query query("www.w3.org", "http");
////	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
////
////	tcp::socket socket(io_svc);
////	boost::asio::connect(socket, endpoint_iterator);
////
//	boost::asio::streambuf request;
//	std::ostream request_stream(&request);
////
////	//request_stream << "GET /NBUStatService/v1/statdirectory/exchange?valcode=" << valcode << "&date=" << date;
//	request_stream << "GET /NBUStatService/v1/statdirectory/ HTTP/1.1\r\nHOST: bank.gov.ua\r\nConnection: close\r\n\r\n";
//
//	//request_stream << "GET /pub/WWW/TheProject.html HTTP/1.1\r\nConnection: Keep_Alive"/* <<
//	/*	"User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)\r\n"
//		"Host : www.tutorialspoint.com\r\n"
//		"Accept - Language : en - us\r\n"
//		"Accept - Encoding : gzip, deflate\r\n"
//		"Connection : Keep-Alive"*/;
//
//	/*if (json)
//	{
//		request_stream << "?json";
//	}*/
//
//	//boost::asio::write(socket, request);
//	boost::asio::write(ssock, request);
//
//	// Read the response status line.
//	boost::asio::streambuf response;
//	//boost::asio::read_until(socket, response, "\r\n");
//	//boost::asio::read_until(socket, response, " ");
//	std::istream response_stream(&response);
//	boost::system::error_code error;
//	//while (boost::asio::read(socket, response,
//	while (boost::asio::read(ssock, response,
//		boost::asio::transfer_at_least(1), error))
//	{
//	}
//	std::string err;
//	if (error != boost::asio::error::eof)
//		err = error.message();
//
//	std::string response_string;
//	while (!response_stream.eof())
//	{
//		std::string temp;
//		std::getline(response_stream, temp);
//
//		response_string.append(temp);
//	}
//
//	return response_string;
//}