#include <iostream>
#include "tinyxml2.h"
#include <crtdbg.h>
#include <windows.h>
#include <memory>
#include <charconv>

#include <limits>
#include <regex>
#include <csignal>
#include <thread>

#include "Library.h"

std::string trim(const std::string& str) {
	// Find the first non-whitespace character
	auto start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos) {
		// If all characters are whitespace, return an empty string
		return "";
	}

	// Find the last non-whitespace character
	auto end = str.find_last_not_of(" \t\n\r\f\v");

	// Create a substring from the first to the last non-whitespace character
	return str.substr(start, end - start + 1);
}

bool workingFlag = true;

void signalHandler(int signal) {
	if (signal == SIGINT || signal == SIGTERM) {
		workingFlag = false;
		std::cout << "Signal received, preparing to terminate program..." << std::endl;
	}
}

int main()
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	Library lib("File.xml");
	
	std::cout << "This is simple implementation of library catalogue, using a tinyxml2" << "\n";
	while (workingFlag) {
		std::cout << "Possible options:\n";
		std::cout << "\t1) List all books and sections\n";
		std::cout << "\t2) List books from one section(by name)\n";
		std::cout << "\t3) Print out info about book by name\n";
		
		std::cout << "\t4) Add a book into existed section\n";
		std::cout << "\t5) Create a new section\n";

		std::cout << "\t6) Delete book by name\n";
		std::cout << "\t7) Delete section by name\n";

		std::cout << "\t8) Rename section\n";
		
		std::cout << "Enter the option(0 - exit): ";
		int opt;
		std::string optStr;
		if (!(std::cin >> optStr))
			continue;
		

		auto res = std::from_chars(optStr.data(), optStr.data() + optStr.size(), opt);
		std::cout << "====================\n";
		if (res.ec == std::errc()) {
			if (opt == 1) {
				lib.listLibrary();
			
			}
			else if (opt == 2) {
				std::cout << "Enter the section name: ";
				std::string sectionName;
				if (!(std::cin >> sectionName)) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				
				if (lib.hasCollection(sectionName)) {
					lib.listSpecialSectionFromLibrary(sectionName);
				}
				else {
					std::cout << "This section does not exist\n";
				}
			}
			else if (opt == 3) {	
				std::cout << "Enter the title of a book:" << std::endl;
				std::string title;
				std::cin.ignore();
				if (!(std::getline(std::cin, title))) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				auto res = lib.findBookByTitle(title);
				if (res.has_value()) {
					std::cout << "++++Book founded, list the info++++\n";
					res.value().listInfo();
				}
				else {
					std::cout << "Error. Book with such a title does not exist\n";
				}
			}
			else if (opt == 4) {
				std::string id;
				std::string title;
				std::string author;
				std::string year;
				std::string publisher;
				std::string description;
				std::string sectionName;

				std::cin.ignore();
				std::cout << "Enter id" << std::endl;
				if (!(std::cin >> id)) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				std::cin.ignore();
				std::cout << "Enter title:" << std::endl;
				if (!(std::getline(std::cin, title)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}	
				std::cin.ignore();
				std::cout << "Enter author:" << std::endl;
				if (!(std::getline(std::cin, author)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}	

				std::cin.ignore();
				std::cout << "Enter year:" << std::endl;
				if (!(std::cin >> year))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				std::cin.ignore();
				std::cout << "Enter publisher:" << std::endl;
				if (!(std::getline(std::cin, publisher)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				std::cin.ignore();
				std::cout << "Enter description:" << std::endl;
				if (!(std::getline(std::cin, description)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				while (true)
				{
					std::cin.ignore();
					std::cout << "Enter section name:" << std::endl;
					if (!(std::getline(std::cin, sectionName)))
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
						continue;
					}
					sectionName = trim(sectionName);
					std::cout << "++++++++++" << sectionName << "++++++\n";
					if (lib.hasCollection(sectionName)) {
						break;
					}
					else {
						std::cout << "This section does not exist\n";
					}
				}
				if (lib.addBookIntoCollection(sectionName, id, title, author, year, publisher, description)) {
					std::cout << "Successfully on adding a book\n";
				}
				else {
					std::cout << "Failed to add a book\n";
				}

			}
			else if (opt == 5) {
				std::string name;
				std::string id;
				std::cin.ignore();
				std::cout << "Enter name: ";
				if(!(std::getline(std::cin, name)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				std::cin.ignore();
				std::cout << "Enter id: ";
				if(!(std::getline(std::cin, id)))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				if (!lib.existSectionWithNameAndId(name, id)) {		
					lib.createNewSection(name, id);
					std::cout << "Inserted successfully\n";
				}
				else {
					std::cout << "Section with name and/or id exist";
				}
			}
			else if (opt == 6) {
				std::string bookName;
				std::cin.ignore();
				std::cout << "Enter book name:" << std::endl;
				if(!(std::getline(std::cin, bookName)))\
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				if (lib.existBook(bookName)) {
					lib.deleteBook(bookName);
					std::cout << "Removed the book...\n";
				}
				else {
					std::cout << "This book does not exist\n";
				}
			}
			else if (opt == 7) {
				std::string name;
				std::cin.ignore();
				std::cout << "Enter the section name: " << std::endl;
				if (!(std::getline(std::cin, name))) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
				if (lib.existSection(name)) {
					lib.deleteSection(name);
					std::cout << "Successfully deleted section\n";
				}
				else {
					std::cout << "Section with such name does not exist\n";
				}
			}
			else if (opt == 8) {
				std::string oldName;
				std::string newName;
				std::cin.ignore();
				std::cout << "Enter the section name: " << std::endl;
				if (!(std::getline(std::cin, oldName))) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}

				if (lib.existSection(oldName))
				{
					std::cin.ignore();
					std::cout << "Enter the new name: " << std::endl;
					if (!(std::getline(std::cin, newName))) {
						std::this_thread::sleep_for(std::chrono::seconds(1));
						continue;
					}

					lib.renameSection(oldName, newName);

					std::cout << "Renamed successfully\n";
				}
				else {
					std::cout << "Section with such a name does not exist\n";
				}
			}
			else {
				std::cout << "Error with input, please choose correct option\n";
			}
				
		}
		else {
			std::cout << "Error with input, please reenter option\n";
		}
		std::cout << "====================\n";





	}



}
