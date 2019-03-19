#pragma once

#include <string>

class Item 
{
public:

public:
	int m_id;					/// The item id.
	std::string m_chat_link;	/// The chat link.
	std::string m_name;			/// The item name.
	std::string m_icon_url;		/// The full icon URL.
	std::string m_description;	/// The item description. (optional)
	std::string m_type;			/// The item type (see below). Possible values: 
	std::string m_rarity;		/// The item rarity. Possible values: 
};