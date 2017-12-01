#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <cassert>
#include "Inventory.h"
#include "../Resources.h"
#include "../Inputs.h"
#include "../Debuger.h"
#include "Scene.h"
#include "../Settings.h"
#include "../SoundManager.hpp"
#include <string>
#include <fstream>
//#include "Game.h"

Inventory::Inventory()
{

	show_inventory = false;
	show_tab = true;
	show_craft_list = false;
	Item* craft1 = new Item("pickaxe1");
	Item* craft2 = new Item("pickaxe2");
	Item* craft3 = new Item("armor1");
	Item* craft4 = new Item("sword1");
    Item* craft5 = new Item("stairs");
    Item* craft6 = new Item("torch");
    Item* craft7 = new Item("A");
	Item* craft8 = new Item("armor2");
	Item* craft9 = new Item("armor3");
	craft1->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft2->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft3->SetSize(SLOT_SIZE-GRID_THICKNESS);
    craft4->SetSize(SLOT_SIZE-GRID_THICKNESS);
    craft5->SetSize(SLOT_SIZE-GRID_THICKNESS);
    craft6->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft7->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft8->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft9->SetSize(SLOT_SIZE-GRID_THICKNESS);
	craft_list [0] = craft1;
	craft_list [1] = craft2;
	craft_list [2] = craft3;
    craft_list [3] = craft4;
    craft_list [4] = craft5;
    craft_list [5] = craft6;
	craft_list [6] = craft7;
	craft_list [7] = craft8;
	craft_list [8] = craft9;
	tab_item_selected = 0;

}

Inventory::~Inventory()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}
bool Inventory::existItemTabNoFull(std::string id_item){
	
	for(int i = 0; i< TAB_SLOTS; ++i){
		Item* current_item = tab[i];
		if(current_item != nullptr){
			if(current_item->id == id_item && current_item->amount < current_item->max_stack_amount) return true;
		}
	}
	return false;
}
bool Inventory::existItemInventoryNoFull(std::string id_item){
	
	for(int i = 0; i< Y_SLOTS; ++i){
		for(int j = 0; j< X_SLOTS; ++j){
			Item* current_item = inventory[i][j];
			if(current_item != nullptr){
				if(current_item->id == id_item && current_item->amount < current_item->max_stack_amount) return true;
			}
	}
	}
	return false;
}


bool Inventory::isInventoryFull(){
	
	for(int i = 0; i< Y_SLOTS; ++i){
		for(int j = 0; j< X_SLOTS; ++j){
			Item* current_item = inventory[i][j];
			if(current_item == nullptr) return false;
		}
	}
	return true;
}
bool Inventory::isTabFull(){
	
	for(int i = 0; i< TAB_SLOTS; ++i){
		Item* current_item = tab[i];
		if(current_item == nullptr) return false;
	}
	return true;
}
int Inventory::stackItemTab(std::string id_item, int amount){
	int i = 0;
    int residue = amount;
	while(i<TAB_SLOTS){
		Item* current_item = tab[i];
		if(current_item != nullptr){
			if(current_item->id == id_item && current_item->amount < current_item->max_stack_amount){
                residue = incrementItem(current_item, amount);
				return residue;
			}
		}
		++i;
	}
    //no existeix
    if(residue>0){
        //tab full
        if(!isTabFull()){
            residue = giveItemTab(id_item, amount);
        }
    }
    return residue;
}
int Inventory::incrementItem(Item* current_item, int amount){
	if(current_item->amount+amount > current_item->max_stack_amount){
		int amount_stacked = current_item->max_stack_amount-current_item->amount;
		current_item->amount = current_item->max_stack_amount;
		return amount-amount_stacked;
	}
	else{
		current_item->amount += amount;
		return 0;
	}
}
int Inventory::decrementItem(std::string id_item, int amount){
	for(int i = 0; i< Y_SLOTS; ++i){
		for(int j = 0; j< X_SLOTS; ++j){
			Item* current_item = inventory[i][j];
			if(current_item != nullptr){
				if(current_item->id == id_item){
					if(current_item->amount - amount <0){
						amount -= current_item->amount;
						delete current_item;
						inventory[i][j] = nullptr;
					}
					else{
						current_item->amount -=amount;
						if(current_item->amount <= 0){
							delete current_item;
							inventory[i][j] = nullptr;
						}
						amount = 0;
					}
				}
			}
		}
	}
	if(amount > 0){
		for(int i = 0; i< TAB_SLOTS; ++i){
			Item* current_item = tab[i];
			if(current_item != nullptr){
				if(current_item->id == id_item){
					if(current_item->amount - amount <0){
						amount -= current_item->amount;
						delete current_item;
						tab[i] = nullptr;
					}
					else{
						current_item->amount -=amount;
						if(current_item->amount <= 0){
							delete current_item;
							tab[i] = nullptr;
						}
						amount = 0;
					}
				}
			}

		}
	}
}
void Inventory::decrementItemAtTab() {
	Item* item = tab[tab_item_selected];
	if (item == nullptr) return;
	item->amount -= 1;
	if (item->amount <= 0) {
		delete item;
		tab[tab_item_selected] = nullptr;
	}
}
int Inventory::stackItemInventory(std::string id_item, int amount){
	//existeix
	if(existItemInventoryNoFull(id_item)){
		for(int i = 0; i< Y_SLOTS; ++i){
			for(int j = 0; j< X_SLOTS; ++j){
				Item* current_item = inventory[i][j];
				if(current_item != nullptr){
					if(current_item->id == id_item && current_item->amount < current_item->max_stack_amount){
						int residue = incrementItem(current_item, amount);
						//sobra

						if(residue == 0) return 0;
						//no sobra
						else return stackItemInventory(id_item, residue);
					}
				}
			}
		}
	}
	//no existeix
	else {
		//inventory full
		if(isInventoryFull()){
			return giveItemTab(id_item, amount);
		}
		//inventory no full
		else{
			return giveItemInventory(id_item, amount);
		}
	}
}

int Inventory::giveItemTab(std::string id, int amount){
	if(isTabFull()) return amount;
	else{
		for(int i = 0; i< TAB_SLOTS; ++i){
			Item* current_item = tab[i];
			if(current_item == nullptr){
				Item* new_item = new Item(id);
				new_item->amount = 0;
				new_item->SetSize(SLOT_SIZE-GRID_THICKNESS);
				bool residue = incrementItem(new_item, amount);
				tab[i] = new_item;
				if(residue == 0) return 0;
				else return giveItemTab(id, residue);
			}
		}
	}
}
int Inventory::giveItemInventory(std::string id_item, int amount){
	if(isInventoryFull()) return amount;
	else{
		for(int i = 0; i< Y_SLOTS; ++i){
			for(int j = 0; j<X_SLOTS; ++j){
				Item* current_item = inventory[i][j];
				if(current_item == nullptr){
					Item* new_item = new Item(id_item);
					new_item->amount = 0;
					new_item->SetSize(SLOT_SIZE-GRID_THICKNESS);
					bool residue = incrementItem(new_item, amount);
					inventory[i][j] = new_item;
					if(residue == 0) return 0;
					else return giveItemInventory(id_item, residue);
				}	
			}
		}
	}
}

int Inventory::stackItem(std::string id, int amount){
	int residue = amount;

	if(existItemTabNoFull(id)) residue = stackItemTab(id, residue);
	if(residue == 0) return 0;
	if(existItemInventoryNoFull(id)) residue = stackItemInventory(id, residue);
	if(residue == 0) return 0;
	if(!isTabFull()) residue = giveItemTab(id, residue);
	if(residue == 0) return 0;
	return giveItemInventory(id, residue);

}
Item* Inventory::getItemTool(){
    return toolItem;
}
Item* Inventory::getItemArmor(){
    return armorItem;
}
Item* Inventory::moveItemTool(Item* it){
    if(it != nullptr && it->type != Item::TOOL) return it;
    Item* it_actual = toolItem;
    toolItem = it;
    return it_actual;
}
Item* Inventory::moveItemArmor(Item* it){
    if(it != nullptr && it->type != Item::ARMOR) return it;
    Item* it_actual = armorItem;
    armorItem = it;
    return it_actual;
}
Item* Inventory::moveItemTab(Item* it, int tab_x){
	Item* it_actual = tab[tab_x];
	if(it == nullptr){
		tab[tab_x] = nullptr;
		return it_actual;
	} 
	else if(it_actual == nullptr || it_actual->id != it->id || (it_actual->id == it->id && it_actual->amount == it_actual->max_stack_amount)){
		tab[tab_x] = it;
		return it_actual;
	} 
	else {
		int residue = incrementItem(it_actual, it->amount);
		if(residue == 0) return nullptr;
		else{
			it->amount = residue;
			return it;
		}
	}
}
Item* Inventory::moveItemInventory(Item* it, int inv_x, int inv_y){
	Item* it_actual = inventory[inv_y][inv_x];
	if(it == nullptr){
		inventory[inv_y][inv_x] = nullptr;
		return it_actual;
	} 
	else if(it_actual == nullptr || it_actual->id != it->id || (it_actual->id == it->id && it_actual->amount == it_actual->max_stack_amount)){
		inventory[inv_y][inv_x] = it;
		return it_actual;
	} 
	else {
		int residue = incrementItem(it_actual, it->amount);
		if(residue == 0) return nullptr;
		else{
			it->amount = residue;
			return it;
		}
	}
}


int Inventory::getTabIndex(sf::Vector2f pos){
	pos.x -= x_tab;
	if(pos.x < 0 ) return -1;
	int size_socket = SLOT_SIZE;
	int tab_x = pos.x/size_socket;
	if(tab_x<TAB_SLOTS && pos.y >= y_tab && pos.y < y_tab+SLOT_SIZE && show_tab) return tab_x;
	else return -1;

}
int Inventory::getCraftIndex(sf::Vector2f pos){
	pos.y -= y_craft_list;
	if(pos.y < 0 ) return -1;
	int size_socket = SLOT_SIZE;
	int craft_y = pos.y/size_socket;
	if(craft_y<N_CRAFT_ITEMS && pos.x >= x_craft_list && pos.x < x_craft_list+SLOT_SIZE && show_craft_list) return craft_y;
	else return -1;

}
int Inventory::getToolIndex(sf::Vector2f pos){
    pos.x -= x_tab;
    if(pos.x < 0 ) return -1;
    int size_socket = SLOT_SIZE;
    int tab_x = pos.x/size_socket;
    if(tab_x==(TAB_SLOTS+1) && pos.y >= y_tab && pos.y < y_tab+SLOT_SIZE && show_tab) return 0;
    else return -1;
}
int Inventory::getArmorIndex(sf::Vector2f pos){
    pos.x -= x_tab;
    if(pos.x < 0 ) return -1;
    int size_socket = SLOT_SIZE;
    int tab_x = pos.x/size_socket;
    if(tab_x==(TAB_SLOTS+2) && pos.y >= y_tab && pos.y < y_tab+SLOT_SIZE && show_tab) return 0;
    else return -1;
}
int Inventory::getTotalAmountInventory(std::string id_item){
	int amount = 0;
	for(int i = 0; i< Y_SLOTS; ++i){
		for(int j = 0; j<X_SLOTS; ++j){
			Item* current_item = inventory[i][j];
			if(current_item != nullptr && current_item->id == id_item){
				amount += current_item->amount;
			}	
		}
	}

	return amount;
}
int Inventory::getTotalAmountTab(std::string id_item){
	int amount = 0;
	for(int i = 0; i< TAB_SLOTS; ++i){
		Item* current_item = tab[i];
		if(current_item != nullptr && current_item->id == id_item){
			amount += current_item->amount;
		}	
		
	}
	return amount;
}

void Inventory::craftItem(std::string id_item){
	Item* new_item = new Item(id_item);
	bool invalid_craft = false;
	if(!isTabFull() || !isInventoryFull()){
		for(auto const &it : new_item->craft_cost) {
			std::string aux_id = it.first;
			int aux_amount_need  = it.second;
			int total_amount = getTotalAmountInventory(aux_id);
			if(total_amount < aux_amount_need) total_amount += getTotalAmountTab(aux_id);
			if(total_amount < aux_amount_need) invalid_craft = true;
		}
		if(!invalid_craft){
			for(auto const &it : new_item->craft_cost) {
				std::string aux_id = it.first;
				int aux_amount_need  = it.second;
				int aux_res = decrementItem(aux_id, aux_amount_need);
			}
			SoundManager::playSoundNoRestart("craft");
			int res = stackItem(id_item,1);
		}

	}


}
std::string Inventory::getIdCraftItem(int index){
	return craft_list[index]->id;
}
std::string Inventory::getIdItemAtTab() {
	return tab[tab_item_selected] != nullptr ? tab[tab_item_selected]->id : "0";
}
Item * Inventory::getItemAtTab() {
	return tab[tab_item_selected];
}

bool Inventory::inventoryClick(float x, float y, std::string key){
	int index_tab = getTabIndex(sf::Vector2f(x,y));
	int index_craft = getCraftIndex(sf::Vector2f(x,y));
    int index_tool = getToolIndex(sf::Vector2f(x,y));
    int index_armor = getArmorIndex(sf::Vector2f(x,y));
	sf::Vector2i index_inventory = getInventoryIndex(sf::Vector2f(x,y));

	//Item* mouseItem = getMouseItem();
	if(key == "mouseLeft"){
		if(index_tab != -1) mouseItem = moveItemTab(mouseItem, index_tab);
		else if(index_inventory != sf::Vector2i(-1,-1)) mouseItem = moveItemInventory(mouseItem, index_inventory.x, index_inventory.y);
        else if(index_tool != -1) mouseItem = moveItemTool(mouseItem);
        else if(index_armor != -1) mouseItem = moveItemArmor(mouseItem);
		else if(index_craft != -1) craftItem(getIdCraftItem(index_craft));
		else if(mouseItem != nullptr) return false;

	} else if(key == "mouseRight"){
		if(index_tab != -1){
			Item* pos_item = tab[index_tab];
			if(mouseItem == nullptr && pos_item != nullptr){
				mouseItem = pos_item->getItemCopy();
				mouseItem->amount = 1;
				
				pos_item->decrement(1);
				if(pos_item->amount <=0) {
					deleteItemTab(index_tab);
				}
			}
			else if(pos_item != nullptr && mouseItem->id == pos_item->id){
				mouseItem->amount+=1;
				pos_item->decrement(1);
				if(pos_item->amount <=0) {
					deleteItemTab(index_tab);
				}
			}
			else if(pos_item == nullptr && mouseItem != nullptr){
				pos_item = mouseItem->getItemCopy();
				pos_item->amount = 1;
				Item* res = moveItemTab(pos_item, index_tab);
				mouseItem->amount-=1;
				if(mouseItem->amount <=0) {
					deleteMouseItem();
				}
			}
		}
		else{
			sf::Vector2i index_inventory = getInventoryIndex(sf::Vector2f(x,y));
			if(index_inventory != sf::Vector2i(-1,-1)){

				Item* pos_item = inventory[index_inventory.y][index_inventory.x];
				if(mouseItem == nullptr && pos_item != nullptr){
					mouseItem = pos_item->getItemCopy();
					mouseItem->amount = 1;
					
					pos_item->decrement(1);
					if(pos_item->amount <=0) {
						deleteItemInventory(index_inventory.x, index_inventory.y);
					}
				}
				else if(pos_item != nullptr && mouseItem->id == pos_item->id){
					mouseItem->amount+=1;
					pos_item->decrement(1);
					if(pos_item->amount <=0) {
						deleteItemInventory(index_inventory.x, index_inventory.y);
					}
				}
				else if(pos_item == nullptr && mouseItem != nullptr){
					pos_item = mouseItem->getItemCopy();
					pos_item->amount = 1;
					Item* res = moveItemInventory(pos_item, index_inventory.x, index_inventory.y);
					mouseItem->amount-=1;
					if(mouseItem->amount <=0) {
						deleteMouseItem();
					}
				}
			}
		}
	}
	if(key == "contr+mouseLeft"){
		if(index_tab != -1){
			Item* pos_item = tab[index_tab];
			if(mouseItem == nullptr && pos_item != nullptr){
				mouseItem = pos_item->getItemCopy();
				mouseItem->amount = pos_item->amount/2;
				
				pos_item->decrement(pos_item->amount/2);
				if(pos_item->amount <=0) {
					deleteItemTab(index_tab);
				}
			}
			else if(pos_item != nullptr && mouseItem->id == pos_item->id){
				int half = pos_item->amount/2;
				if(mouseItem->amount+half > mouseItem->max_stack_amount){
					half = mouseItem->max_stack_amount-mouseItem->amount;
				}
				mouseItem->amount+=half;
				pos_item->decrement(half);
				if(pos_item->amount <=0) {
					deleteItemTab(index_tab);
				}
			}
			else if(pos_item == nullptr && mouseItem != nullptr){
				pos_item = mouseItem->getItemCopy();
				pos_item->amount = mouseItem->amount/2;
				mouseItem->decrement(mouseItem->amount/2);
				Item* res = moveItemTab(pos_item, index_tab);
				if(mouseItem->amount <=0) {
					deleteMouseItem();
				}
			}
		}
		else{
			sf::Vector2i index_inventory = getInventoryIndex(sf::Vector2f(x,y));
			if(index_inventory != sf::Vector2i(-1,-1)){
				Item* pos_item = inventory[index_inventory.y][index_inventory.x];
				if(mouseItem == nullptr && pos_item != nullptr){
					mouseItem = pos_item->getItemCopy();
					mouseItem->amount = pos_item->amount/2;
					
					pos_item->decrement(pos_item->amount/2);
					if(pos_item->amount <=0) {
						deleteItemInventory(index_inventory.x, index_inventory.y);
					}
				}
				else if(pos_item != nullptr && mouseItem->id == pos_item->id){
					int half = pos_item->amount/2;
				if(mouseItem->amount+half > mouseItem->max_stack_amount){
					half = mouseItem->max_stack_amount-mouseItem->amount;
				}
				mouseItem->amount+=half;
				pos_item->decrement(half);
					if(pos_item->amount <=0) {
						deleteItemInventory(index_inventory.x, index_inventory.y);
					}
				}
				else if(pos_item == nullptr && mouseItem != nullptr){
					pos_item = mouseItem->getItemCopy();
					pos_item->amount = mouseItem->amount/2;
					mouseItem->decrement(mouseItem->amount/2);
					Item* res = moveItemInventory(pos_item, index_inventory.x, index_inventory.y);
					if(mouseItem->amount <=0) {
						deleteMouseItem();
					}
				}
			}
		}
	} else if(key == "shift+mouseLeft"){
		if(index_tab != -1){
			Item* pos_item = tab[index_tab];
            if(pos_item != nullptr){
                int residue = pos_item->amount;
                residue = stackItemInventory(pos_item->id, residue);
                pos_item->amount = residue;
                if(residue<=0) deleteItemTab(index_tab);
            }

		}
		else{
			sf::Vector2i index_inventory = getInventoryIndex(sf::Vector2f(x,y));
			if(index_inventory != sf::Vector2i(-1,-1)){
				Item* pos_item = inventory[index_inventory.y][index_inventory.x];
                if(pos_item != nullptr) {
                    int residue = pos_item->amount;
                    residue = stackItemTab(pos_item->id, residue);
                    pos_item->amount = residue;
                    if (residue <= 0) deleteItemInventory(index_inventory.x, index_inventory.y);
                }
			}
		}
	}
	return true;
}
void Inventory::deleteMouseItem(){
	delete mouseItem;
	mouseItem = nullptr;
}
void Inventory::deleteItemTab(int x){
	delete tab[x];
	tab[x] = nullptr;
}
void Inventory::deleteItemInventory(int x, int y){
	delete inventory[y][x];
	inventory[y][x] = nullptr;
}
sf::Vector2i Inventory::getInventoryIndex(sf::Vector2f pos){
	pos.x -= x_inventory;
	pos.y -= y_inventory;
	if(pos.x < 0 || pos.y < 0) return sf::Vector2i(-1,-1);
	int size_socket = SLOT_SIZE;
	int inv_x = pos.x/size_socket;
	int inv_y = pos.y/size_socket;
	if(inv_x<X_SLOTS && inv_y < Y_SLOTS && show_inventory) return sf::Vector2i(inv_x, inv_y);
	else return sf::Vector2i(-1,-1);
} 
void Inventory::Draw(sf::RenderWindow& renderWindow)
{

	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("resources/font1.ttf"))
	{
		std::cout << "font error" << std::endl;
	}
	text.setCharacterSize(13);
	text.setColor(sf::Color::Red);
	//text.setStyle(sf::Text::Bold);
	text.setFont(font); // font is a sf::Font
	char c[10];


	//std::cout << x_inventory << " " << y_inventory << std:: endl;
	sf::View currentView = renderWindow.getView();
	sf::Vector2f centerView = currentView.getCenter();
	sf::Vector2f sizeView = currentView.getSize();
	x_inventory = centerView.x-SLOT_SIZE*X_SLOTS/2;
	y_inventory = centerView.y-SLOT_SIZE*Y_SLOTS/2;
	x_tab = centerView.x-SLOT_SIZE*TAB_SLOTS/2;
	y_tab = centerView.y+sizeView.y/2 - SLOT_SIZE*1.5;
	x_craft_list = centerView.x-sizeView.x/2+SLOT_SIZE; 
	y_craft_list = centerView.y-SLOT_SIZE*Y_SLOTS/2;

	sf::RectangleShape rectangle(sf::Vector2f(0, 0));

				
	rectangle.setFillColor(sf::Color(255, 211, 124));
	rectangle.setOutlineThickness(GRID_THICKNESS);
	rectangle.setOutlineColor(sf::Color(147, 91, 0));
	rectangle.setSize(sf::Vector2f(SLOT_SIZE-GRID_THICKNESS, SLOT_SIZE-GRID_THICKNESS));

	if(show_inventory){
		for(int i = 0; i<Y_SLOTS; i = ++i){
			for(int j = 0; j<X_SLOTS; ++j){
				rectangle.setPosition(sf::Vector2f(j*SLOT_SIZE+x_inventory, i*SLOT_SIZE+y_inventory));
				renderWindow.draw(rectangle);
			}
		}
		for(int i = 0; i<Y_SLOTS; ++i){
			for(int j = 0; j<X_SLOTS; ++j){
				Item* item = inventory[i][j];
				if(item != nullptr){
					item->SetPosition(j*SLOT_SIZE+x_inventory, i*SLOT_SIZE+y_inventory);

					int test = item->amount;
					sprintf(c, "%i", test);
					std::string string(c);
					sf::String str(string);
					text.setString(str);
					item->Draw(renderWindow, text);




				}
				
			}
		}
	}
	if(show_tab){
		for(int i = 0; i<TAB_SLOTS; i = ++i){
			rectangle.setPosition(sf::Vector2f(i*SLOT_SIZE+x_tab, y_tab));
			renderWindow.draw(rectangle);
		}
        TextureManager *t = Resources::getTextureManager("tileMap");


        rectangle.setPosition(sf::Vector2f((TAB_SLOTS+1)*SLOT_SIZE+x_tab, y_tab));
        renderWindow.draw(rectangle);
        sf::Sprite s1;
        sf::Vector2f item_pos1(sf::Vector2f((TAB_SLOTS+1)*SLOT_SIZE+x_tab, y_tab));
        t->generateSprite("tool", item_pos1, s1, sf::Vector2f(SLOT_SIZE,SLOT_SIZE));
        renderWindow.draw(s1);
        rectangle.setPosition(sf::Vector2f((TAB_SLOTS+2)*SLOT_SIZE+x_tab, y_tab));
        renderWindow.draw(rectangle);
        sf::Sprite s2;
        sf::Vector2f item_pos2(sf::Vector2f((TAB_SLOTS+2)*SLOT_SIZE+x_tab, y_tab));
        t->generateSprite("armor", item_pos2, s2, sf::Vector2f(SLOT_SIZE,SLOT_SIZE));
        renderWindow.draw(s2);
		for(int i = 0; i<TAB_SLOTS; ++i){

			Item* item = tab[i];
			if(item != nullptr) {
				//std::cout << "haha" << std::endl;
				item->SetPosition(i*SLOT_SIZE+x_tab, y_tab);
				int test = item->amount;
				sprintf(c, "%i", test);
				std::string string(c);
				sf::String str(string);
				text.setString(str);
				item->Draw(renderWindow, text);

			}
		}
		sf::RectangleShape selected(rectangle);
		selected.setPosition(sf::Vector2f(tab_item_selected*SLOT_SIZE+x_tab, y_tab));
		selected.setFillColor(sf::Color::Transparent);
		selected.setOutlineThickness(GRID_SELECTED_THICKNESS);
		selected.setOutlineColor(sf::Color(210, 160, 70));
		renderWindow.draw(selected);

        Item* item = toolItem;
        if(item != nullptr) {
            item->SetPosition((TAB_SLOTS+1)*SLOT_SIZE+x_tab, y_tab);
            std::string string = "";
            sf::String str(string);
            text.setString(str);
            item->Draw(renderWindow, text);


        }
        item = armorItem;
        if(item != nullptr) {
            item->SetPosition((TAB_SLOTS+2)*SLOT_SIZE+x_tab, y_tab);
            std::string string = "";
            sf::String str(string);
            text.setString(str);
            item->Draw(renderWindow, text);
        }
	}
	if(show_craft_list){
		
		for(int i = 0; i<N_CRAFT_ITEMS; i = ++i){
			Item* item = craft_list[i];
			rectangle.setPosition(sf::Vector2f(x_craft_list, i*SLOT_SIZE+y_craft_list));
			renderWindow.draw(rectangle);
			item->SetPosition(x_craft_list, i*SLOT_SIZE+y_craft_list);

			int test = 1;
			sprintf(c, "%i", test);
			std::string string(c);
			sf::String str(string);
			text.setString(str);
			item->Draw(renderWindow, text);


			int j=0;
			for(auto const &it : item->craft_cost) {
				Item* item2 = new Item(it.first);
				item2->amount = it.second;
				item2->SetPosition(x_craft_list+SLOT_SIZE+GRID_THICKNESS+j*SLOT_SIZE/2+j*GRID_THICKNESS, i*SLOT_SIZE+y_craft_list);
				item2->SetSize(SLOT_SIZE/2);

				int test = item2->amount;
				sprintf(c, "%i", test);
				std::string string(c);
				sf::String str(string);
				text.setString(str);
				item2->Draw(renderWindow, text);
				++j;


			}
						
		}
		
	}
	if(mouseItem != nullptr){
		sf::Vector2f position = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
		mouseItem->SetPosition(position.x-SLOT_SIZE/2, position.y-SLOT_SIZE/2);


		int test = mouseItem->amount;
		sprintf(c, "%i", test);
		std::string string(c);
		sf::String str(string);
		text.setString(str);
		mouseItem->Draw(renderWindow, text);
	}
}



void Inventory::Update(sf::RenderWindow &window)
{
	sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));



	//_position.x += centerView.x-sizeView.x/2;
	//_position.y += centerView.y-sizeView.y/2;



	if(Inputs::MouseBreak(Inputs::M_LEFT)){
		if(Inputs::KeyDown(Inputs::L_CONTR)) inventoryClick(position.x, position.y, "contr+mouseLeft");
		else if(Inputs::KeyDown(Inputs::L_SHIFT)) inventoryClick(position.x, position.y, "shift+mouseLeft");
		else {
			if(!inventoryClick(position.x, position.y, "mouseLeft")){
				Player *p = Scene::getScene()->getPlayer();
				Map *map = Scene::getScene()->getMap();
				int chunkE = map->getChunkIndex(p->GetPosition().x);
				int index_chunk = map->getIndexMatChunk(chunkE);
				if(index_chunk != -1) {
					SoundManager::playSoundNoRestart("bubble");
					map->_chunk_mat[index_chunk]->addFallingTile(mouseItem->id,mouseItem->id,p->GetPosition(),Settings::TILE_SIZE);
				}
				mouseItem->amount -=1;
				if(mouseItem->amount <=0) deleteMouseItem();
			}
		}
	}
	else if(Inputs::MouseBreak(Inputs::M_RIGHT)){
		inventoryClick(position.x, position.y, "mouseRight");
	}
	if(Inputs::KeyBreak(Inputs::Q)&& !Debuger::isTerminalActive()){
		if(show_inventory) {
			show_inventory = false;
			show_craft_list = false;
		}
		else {
			show_inventory = true;
			show_craft_list = true;
		}
	}

	int tab_selection_delta = Inputs::GetWheel();
	tab_item_selected = (tab_item_selected - tab_selection_delta);
	while (tab_item_selected < 0) {
		tab_item_selected += TAB_SLOTS;
	}
	tab_item_selected %= TAB_SLOTS;

	for (unsigned int i = 1; i <= Inventory::TAB_SLOTS; ++i) {
		if (Inputs::KeyDown(static_cast<Inputs::Key>(Inputs::Key::N0 +i))) tab_item_selected = i-1;
	}
}

void Inventory::saveData(std::ofstream &myfile){


	//std::string x = std::to_string(_sprite.getPosition().x);
	//std::string y = std::to_string(_sprite.getPosition().y);
	myfile << X_SLOTS <<" " << Y_SLOTS << "\n";
	for(int i=0; i<Y_SLOTS; i++){
		for(int j=0; j<X_SLOTS; j++){
			Item *it =inventory[i][j];
			if(it==nullptr) myfile << -1 << " " << -1 << " ";
			else myfile << it->id << " " << it->amount << " ";
		}
	}
	myfile << "\n" << TAB_SLOTS << "\n";
	for(int i=0; i<TAB_SLOTS; i++){
		Item *it =tab[i];
		if(it==nullptr) myfile << -1 << " " << -1 << " ";
		else myfile << it->id << " " << it->amount << " ";

	}
    myfile << "\n";
    Item *it =toolItem;
    if(it==nullptr) myfile << -1 << " " << -1 << " ";
    else myfile << it->id << " " << it->amount << " ";
    it =armorItem;
    if(it==nullptr) myfile << -1 << " " << -1 << " ";
    else myfile << it->id << " " << it->amount << " ";
}
void Inventory::loadData(std::ifstream &myfile){

	std::string x_sl;
	std::string y_sl;
	myfile >> x_sl >> y_sl;
	for(int i=0; i< stoi(y_sl); i++){
		for(int j=0; j<stoi(x_sl); j++){
			std::string item;
			std::string amount;
			myfile >> item >> amount;
			if(item != "-1"){
				Item *it = new Item(item);
				it->amount = stoi(amount);
                it->SetSize(SLOT_SIZE-GRID_THICKNESS);
				inventory[i][j] = it;
			}
		}
	}

	std::string n_tab;
	myfile >> n_tab;
	for(int i=0; i< stoi(n_tab); i++){

		std::string item;
		std::string amount;
		myfile >> item >> amount;
		if(item != "-1"){
			Item *it = new Item(item);
			it->amount = stoi(amount);
            it->SetSize(SLOT_SIZE-GRID_THICKNESS);
			tab[i] = it;
		}

	}
    std::string item;
    std::string amount;
    myfile >> item >> amount;
    if(item != "-1") {
        Item *it = new Item(item);
        it->amount = stoi(amount);
        it->SetSize(SLOT_SIZE-GRID_THICKNESS);
        toolItem = it;
    }
    myfile >> item >> amount;
    if(item != "-1") {
        Item *it = new Item(item);
        it->amount = stoi(amount);
        it->SetSize(SLOT_SIZE-GRID_THICKNESS);
        armorItem = it;
    }
}