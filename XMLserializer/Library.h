#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <optional>

#include "tinyxml2.h"

class Library
{
private:
	class Book {
	private:
		std::string id_;
		std::string title_;
		std::string author_;
		std::string year_;
		std::string publisher_;
		std::string description_;
	public:
		Book(std::string id, std::string title, std::string author, std::string year, std::string publisher, std::string description) :
			id_(id), title_(title), author_(author), year_(year), publisher_(publisher), description_(description) {}

		void listInfo() const {
			std::cout << "Book id: " << id_ << "\n"
				"\t Title: " << title_ << "\n"
				"\t Author: " << author_ << "\n"
				"\t Year: " << year_ << "\n"
				"\t Publisher: " << publisher_ << "\n"
				"\t Description: " << description_ << "\n";
		}

		bool operator==(const Book& other) {
			if (this->title_ == other.title_)
				return true;
			return false;
		}

		bool operator==(const std::string& otherTitle) const {
			return title_ == otherTitle;
		}

		std::string Title() const {
			return title_;
		}
	};
	
	class Section {
	private:
		std::string name_;
		std::string id_;
	public:
		std::vector<Book> Books;
		Section() {};
		Section(std::string name, std::string id) : name_(name), id_(id){};
		
		Section(const Section& other) {
			this->name_ = other.name_;
			this->id_ = other.id_;
			Books.reserve(other.Books.size());
			for(Book var : Books)
			{
				Books.push_back(var);
			}
			
		}

		Section& operator=(const Section& other) {
			if (this == &other) {
				return *this;
			}
			this->name_ = other.name_;
			this->id_ = other.id_;
			Books.reserve(other.Books.size());
			for (Book var : Books)
			{
				Books.push_back(var);
			}
			return *this;
		}

		void listSectionInfo() const {
			std::cout << "+Section name: " << name_ << "\n";
			for (Book book : Books) {
				book.listInfo();
			}
		}

		void booksInSection() const {
			std::cout << "The section name: " << name_ << "\n";
			std::cout << "The books in the section: " << Books.size() << "\n";
			
		}

		std::string Name() const {
			return name_;
		}

		std::string Id() const {
			return id_;
		}

	};
	tinyxml2::XMLDocument document;
	std::map<std::string, Section> Sections;
	std::string path_;
public:
	size_t collectionCounter() const;

	Library(std::string path) : path_(path){
		tinyxml2::XMLError error = document.LoadFile("File.xml");
		if (tinyxml2::XML_SUCCESS != error) {
			std::cout << "Error an open file:" << document.ErrorStr() << "\n";
			Sections.clear();	
		}
		else {
			tinyxml2::XMLElement* root = document.FirstChildElement("library");
			if (nullptr != root) {
				tinyxml2::XMLElement* section = root->FirstChildElement("section");
				while (section != 0) {
					const char* sectionName = section->Attribute("name");
					const char* sectionId = section->Attribute("id");
					Sections[sectionName] = Section(sectionName, sectionId);

					tinyxml2::XMLElement* book = section->FirstChildElement("book");
					while (nullptr != book) {
						const char* bookId = book->Attribute("id");
						const char* title;
						const char* author;
						const char* year;
						const char* publisher;
						const char* description;
						
						tinyxml2::XMLElement* titleElement = book->FirstChildElement("title");
						if(titleElement)
							title = titleElement->GetText();

						tinyxml2::XMLElement* authorElement = book->FirstChildElement("author");
						if (authorElement)
							author = authorElement->GetText();
						
						tinyxml2::XMLElement* yearElement = book->FirstChildElement("year");
						if (yearElement)
							year = yearElement->GetText();

						tinyxml2::XMLElement* publisherElement = book->FirstChildElement("publisher");
						if (publisherElement)
							publisher = publisherElement->GetText();

						tinyxml2::XMLElement* descriptionElement = book->FirstChildElement("description");
						if (descriptionElement)
							description = (descriptionElement->GetText()) ? descriptionElement->GetText() : "N/A";

						Sections[sectionName].Books.emplace_back(bookId, title, author, year, publisher, description);

						book = book->NextSiblingElement("book");
					}
					section = section->NextSiblingElement("section");
					//sections end
				}
			}
		}
		
	}

	void listLibrary() const;
	bool hasCollection(std::string key) const;
	void listSpecialSectionFromLibrary(std::string sectionName) const;
	std::optional<Book> findBookByTitle(const std::string& title) const;
	bool addBookIntoCollection(std::string sectionName, std::string id, std::string title, std::string author, std::string year, std::string publisher, std::string description);
	bool existSectionWithNameAndId(std::string name, std::string id) const;
	void createNewSection(std::string name, std::string id);
	bool existBook(std::string title) const;
	void deleteBook(std::string title);
	bool existSection(std::string name) const;
	void deleteSection(std::string name);
	void renameSection(std::string oldName, std::string newName);

	~Library() {
		std::cout << "Write into a file\n";
		tinyxml2::XMLError error = document.SaveFile(path_.c_str());
		if (error != tinyxml2::XML_SUCCESS)
			std::cout << "Error with an writing\n";
			
	}

};

