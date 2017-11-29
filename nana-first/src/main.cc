#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>

int main(int argc, char* argv[])
{
  using namespace nana;

  form fm;

  //Define a label and display a text.
  label lab{fm, "Hello, <bold blue size=16>Nana C++ Library</>"};
  lab.format(true);

  //Define a button and answer the click event.
  button btn{fm, "Quit"};
  btn.events().click([&fm] {
    fm.close();
  });

	listbox lsbox(fm);
	lsbox.scheme().header_bgcolor = nana::colors::yellow;
	lsbox.scheme().header_grabbed = nana::colors::chocolate;
	lsbox.append_header("Name");
	lsbox.append_header("Age");
	auto cat = lsbox.at(0); //access the default category.
	cat.append({ "Jack", "20" });	//Insert an item
	cat.push_back("Rose");		//Insert another item
	cat.back().text(1, "21");

  //Layout management
  fm.div("vert <><<><weight=80% text><>><listbox><weight=24<><button><>><>");
  fm["text"] << lab;
	fm["listbox"] << lsbox;
	fm["button"] << btn;
  fm.collocate();

	API::refresh_window(lsbox);

  //Show the form
  fm.show();

  //Start to event loop process, it blocks until the form is closed.
  exec();

  return 0;
}
