#if 1
#include <nana/gui.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <string>
#include <future>
#include <chrono>
#include "console.h"
#include <iostream>


constexpr const size_t BufferSize = 1024;


using namespace nana;

class notepad_form : public form
{
public:
	notepad_form()
		: form(API::make_center(800, 600))
	{
		caption("Tap");
		textbox_.borderless(true);
		textbox_.line_wrapped(true);
		API::effects_edge_nimbus(textbox_, effects::edge_nimbus::none);
		_m_make_menus();

		place_.div("vert<menubar weight=28><textbox>");
		place_["menubar"] << menubar_;
		place_["textbox"] << textbox_;
		place_.collocate();
	}


	friend notepad_form& operator <<(notepad_form& self, const std::string& str)
	{
		self.textbox_.append(str, false);
		return self;
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




template<typename T>
bool is_ready(std::future<T> const& fu)
{
	return fu.wait_for(std::chrono::seconds(0))
		== std::future_status::ready;
}


template<typename OUTPUT>
void input_loop(OUTPUT& out, std::future<bool>& quit_flag)
{
	nonblocking_stdin_t<BufferSize> std_in;

	while (true)
	{
		auto in = std_in.get_input();
		if (!in.empty())
			out << in;

		if (is_ready(quit_flag) && quit_flag.get()) 
		{
			std::cout << "Goodbye!" << std::endl;
			return;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
#endif
