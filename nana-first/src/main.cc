#include <nana/gui.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream>
#include <string>
#include <future>
#include <chrono>

#ifdef _WIN32
#else
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#endif




template<typename T>
bool is_ready(std::future<T> const& fu)
{ 
	return fu.wait_for(std::chrono::seconds(0)) 
		== std::future_status::ready;
}



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



template<const size_t BufferSize = 256lu>
struct stdin_t
{
	stdin_t()
		: buff_{0}
	{
  	::fcntl(0, F_SETFL, ::fcntl(0, F_GETFL) | O_NONBLOCK);
	}


std::string get_input()
{
  const auto bytes_read = ::read(0, buff_, sizeof(buff_) - 1);
  if(bytes_read > 0)
  	return buff_;
  return "";
}


private:
	char buff_[BufferSize];
};



template<typename OUTPUT>
void input_loop(OUTPUT& out, std::future<bool>& quit_flag)
{
	stdin_t<> stdin;

	while(true)
	{
		auto in = stdin.get_input();
		if(!in.empty())
			out.append(in);

		if (is_ready(quit_flag) && quit_flag.get()) {
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
