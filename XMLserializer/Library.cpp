#include "Library.h"

size_t Library::collectionCounter() const
{
    return Sections.size();
}

void Library::listLibrary() const
{
	for (auto it = Sections.begin(); it != Sections.end(); ++it) {
		(*it).second.listSectionInfo();
	}
}

bool Library::hasCollection(std::string key) const
{
	auto it = Sections.find(key);
	if (it == Sections.end())
		return false;
	return true;
}

void Library::listSpecialSectionFromLibrary(std::string sectionName) const
{
	 Sections.at(sectionName).listSectionInfo();
}

std::optional<Library::Book> Library::findBookByTitle(const std::string& title) const
{
	std::optional<Library::Book> opt;
	for (auto it = Sections.begin(); it != Sections.end(); ++it) {
		auto res = std::find( (*it).second.Books.begin(), (*it).second.Books.end(), title);
		if (res != (*it).second.Books.end()) {
			opt = *res;
			return opt;
		}
	}
	return opt;
}

bool Library::addBookIntoCollection(std::string sectionName, std::string id, std::string title, std::string author, std::string year, std::string publisher, std::string description)
{
	Sections[sectionName].Books.emplace_back(id, title, author, year, publisher, description);
	tinyxml2::XMLElement* sectionForBook = document.FirstChildElement("library")->FirstChildElement("section");
	while (sectionForBook) {
		if (sectionForBook->Attribute("name") == sectionName) {
			tinyxml2::XMLElement* newBook = document.NewElement("book");
			
			newBook->SetAttribute("id", id.c_str());
			
			tinyxml2::XMLElement* titleElem = document.NewElement("title");
			titleElem->SetText(title.c_str());
			
			tinyxml2::XMLElement* authorElem = document.NewElement("author");
			authorElem->SetText(author.c_str());

			tinyxml2::XMLElement* yearElement = document.NewElement("year");
			yearElement->SetText(year.c_str());

			tinyxml2::XMLElement* publisherElement = document.NewElement("publisher");
			publisherElement->SetText(publisher.c_str());

			tinyxml2::XMLElement* descriptionElement= document.NewElement("description");
			descriptionElement->SetText(description.c_str());

			newBook->InsertEndChild(titleElem);
			newBook->InsertEndChild(authorElem);
			newBook->InsertEndChild(yearElement);
			newBook->InsertEndChild(publisherElement);
			newBook->InsertEndChild(descriptionElement);
			sectionForBook->InsertEndChild(newBook);
			break;
		}
		else {
			sectionForBook = sectionForBook->NextSiblingElement("section");
		}
	}
	return true;
}

bool Library::existSectionWithNameAndId(std::string name, std::string id) const
{
	auto it = std::find_if(Sections.begin(), Sections.end(), [&](std::pair<std::string, Library::Section> section) {
		if (section.second.Name() == name || section.second.Id() == id)
			return true;
		return false;
		});

	if (it == Sections.end())
		return false;
	return true;
	
}

void Library::createNewSection(std::string name, std::string id)
{
	Sections[name] = Section(name, id);
	tinyxml2::XMLElement* newSection = document.NewElement("section");
	newSection->SetAttribute("name", name.c_str());
	newSection->SetAttribute("id", id.c_str());
	
	tinyxml2::XMLElement* libElement = document.FirstChildElement("library");
	libElement->InsertEndChild(newSection);
}

bool Library::existBook(std::string title) const
{
	for (auto it = Sections.begin(); it != Sections.end(); ++it) {
		for (auto book : (*it).second.Books) {
			if (book.Title() == title) {
				return true;
			}
		}
	}
	return false;
}

void Library::deleteBook(std::string title)
{
	/*auto it = std::find_if(Sections.begin(), Sections.end(), [&](std::pair<std::string, Library::Section> kvp) {
		auto i = std::find_if(kvp.second.Books.begin(), kvp.second.Books.end(), [&](Book book) {
			return book.Title() == title;
			});
		if (i != kvp.second.Books.end())
			return true;
		return false;
		});*/
	std::string name;
	for (auto it = Sections.begin(); it != Sections.end(); ++it) {
		for (auto book : (*it).second.Books) {
			if (book.Title() == title) {
				name = (*it).second.Name();
				break;
			}
		}
	}


	for (ptrdiff_t i = 0; i < Sections[name].Books.size(); ++i) {
		if (Sections[name].Books[i].Title() == title) {
			Sections[name].Books.erase(Sections[name].Books.begin() + i);
			break;
		}
	}
	tinyxml2::XMLElement* lib = document.FirstChildElement("library");
	for (tinyxml2::XMLElement* section = lib->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) {
		std::string secName = section->Attribute("name");
		if (secName == name) {
			for (tinyxml2::XMLElement* bookToDel = section->FirstChildElement("book"); bookToDel; bookToDel = bookToDel->NextSiblingElement("book")) {
				std::string ttl = bookToDel->FirstChildElement("title")->GetText();
				if (ttl == title) {
					section->DeleteChild(bookToDel);
					break;
				}
			}
		}
	}
}

bool Library::existSection(std::string name) const
{
	auto it = std::find_if(Sections.begin(), Sections.end(), [&](std::pair<std::string, Library::Section> kvp) {
		if (kvp.second.Name() == name)
			return true;
		});
	if (it != Sections.end())
		return true;
	return false;
}

void Library::deleteSection(std::string name)
{
	tinyxml2::XMLElement* library = document.FirstChildElement("library");
	for (tinyxml2::XMLElement* sectionToDelete = library->FirstChildElement("section"); sectionToDelete; sectionToDelete = sectionToDelete->NextSiblingElement("section")) {
		std::string name_ = sectionToDelete->Attribute("name");
		if (name_ == name)
		{
			library->DeleteChild(sectionToDelete);
			break;
		}
	}
}


void Library::renameSection(std::string oldName, std::string newName)
{
	tinyxml2::XMLElement* library = document.FirstChildElement("library");
	for (tinyxml2::XMLElement* section = library->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) {
		std::string name_ = section->Attribute("name");
		if (name_ == oldName)
		{
			section->SetAttribute("name", newName.c_str());
			return;
		}
	}
}


