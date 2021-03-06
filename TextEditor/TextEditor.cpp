// TextEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//use namespaces
using namespace std;
using namespace nana;

int main(int argc, char* argv[])
{
	//variable to store the filename
	string filename = "";

	//create the basic window
	form fm;
	fm.caption("Text Editor"); //set title
	fm.size(nana::size{ 800, 600 }); //and window size (width, height)
	
	//create the textbox
	textbox txtBox{ fm, "" }; // (form, start text)
	
	//create a menubar
	menubar mn(fm);
	//add the main menu options
	mn.push_back("File");
	mn.push_back("Edit");

	//The submenu options for the "File Menu" (0th menu)

	mn.at(0).append("New", [&txtBox, &filename](menu::item_proxy& ip) {
		filename = ""; //reset filename
		txtBox.caption(""); //and textbox
	});

	//Open an existing file
	mn.at(0).append("Open", [&txtBox, &filename](menu::item_proxy& ip) {
		filebox fb(true); //a file dialog (is open filebox)
		
		//add filter
		fb.add_filter("Text files", "*.txt"); // (Filter name, filter)
		fb.add_filter("All files", "*.*");

		//once "ok" has been clicked in the dialog
		if (fb()) {
			filename = fb.file(); //get the choosen file
			txtBox.load(filename); //and load it into the textbox
			
			//NOTE: You can also use ifstream to load a file
		}

	});
	
	//Save a file
	mn.at(0).append("Save", [&txtBox, &filename] (menu::item_proxy& ip) {
		//if a file is already open
		if (filename != "") {
			txtBox.store(filename); //put text in textbox in the file

			//NOTE: You can also use an ofstream to store the data
		} else { //if no file is currently open
			filebox fb(false); //create a file dialog (false, so we're saving, not opening)
			fb.add_filter("Text files", "*.txt"); //add filters
			fb.add_filter("All files", "*.*");

			//launch the dialog and wait for OK to be clicked
			if (fb()) {
				filename = fb.file(); //get filename
				txtBox.store(filename); //and store data in our file
			}
		}
	});

	//Ditto of above
	mn.at(0).append("Save As", [&txtBox, &filename](menu::item_proxy& ip) {
		filebox fb(false);
		fb.add_filter("Text files", "*.txt");
		fb.add_filter("All files", "*.*");

		if (fb()) {
			filename = fb.file();
			txtBox.store(filename);
		}
	});

	mn.at(0).append("Exit", [&fm](menu::item_proxy& ip) {
		fm.close(); //close the form
	});
	
	//Copy currently selected text
	mn.at(1).append("Copy", [&txtBox](menu::item_proxy& ip) {
		txtBox.copy();
	});

	//Paste text on clipboard
	mn.at(1).append("Paste", [&txtBox](menu::item_proxy& ip) {
		txtBox.paste();
	});

	//Cut selected text
	mn.at(1).append("Cut", [&txtBox](menu::item_proxy& ip) {
		txtBox.copy();
		txtBox.del();
	});

	//create a place called layout for our form
	place layout(fm);
	
	//allocate the space to our widgets (see nanapro.org docs for more info)
	layout.div("<vertical <menu weight=30><textbox weight=550 margin=[5, 10, 5, 10]>>");
	layout["menu"] << mn;
	layout["textbox"] << txtBox;
	layout.collocate();

	//show the form and execute main loop
	fm.show();
	exec();

    return 0;
}

