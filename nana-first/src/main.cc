#include <nana/gui.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream>
#include <string>
#include <future>
#ifdef _WIN32
#include <conio.h>
#endif


using namespace nana;


class notepad_form : public form
{
public:
	notepad_form()
	{
		caption("Tap");
		textbox_.borderless(true);
		API::effects_edge_nimbus(textbox_, effects::edge_nimbus::none);
		_m_make_menus();

		place_.div("vert<menubar weight=28><textbox>");
		place_["menubar"] << menubar_;
		place_["textbox"] << textbox_;
		place_.collocate();
	}

	
	void append(const std::string& str)
	{
		textbox_.append(str, false);
	}


private:
	void _m_make_menus()
	{
		menubar_.push_back("&FILE");
		menubar_.at(0).append("E&xit", [this](menu::item_proxy&) {
			close();
		});
	}


	place   place_{ *this };
	menubar menubar_{ *this };
	textbox textbox_{ *this };
};



template<typename OUTPUT>
void input_loop(OUTPUT& out, std::future<bool>& quit_flag)
{
	char buffer[512] = { 0 };
	auto point = 0;

	while(true)
	{
		while (_kbhit())
		{
			auto cur = _getch();
			if (point > 511) point = 511;
			if (cur != 13) buffer[point++] = cur;
			else {
				buffer[point] = '\0';
				point = 0;
				out.append(buffer + '\n');
			}
		}

		if (quit_flag._Is_ready() && quit_flag.get()) {
			std::cout << "Goodbye!" << std::endl;
			return;
		}
	}
}


int main()
{
	notepad_form npform;
	npform.show();

	std::promise<bool> quit_flag;
	auto fu1 = quit_flag.get_future();
	auto fu2 = std::async(std::launch::async, input_loop<notepad_form>, std::ref(npform), std::ref(fu1));
	
	exec(); //main thread blocks till gui is closed

	quit_flag.set_value(true); //signal child to terminate
	fu2.get(); //wait for child

	return 0;
}
