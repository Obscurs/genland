
#include "Item.h"
#include "Inputs.h"
class Inventory
{
public:	
	Inventory();
	~Inventory();

	const static int X_SLOTS = 10;
	const static int Y_SLOTS = 5;
	const static int TAB_SLOTS = 8;
	const static int SLOT_SIZE = 50;
	const static int GRID_THICKNESS = 2;
	const static int N_CRAFT_ITEMS = 2;

	
	
	
	void Draw(sf::RenderWindow& renderWindow);
	void Update(Inputs &inputs, sf::RenderWindow &window);
	int stackItem(std::string id, int amount);
	bool show_inventory;
	bool show_tab;
	bool show_craft_list;


private:

	
	int giveItemTab(std::string id, int amount);
	int giveItemInventory(std::string id, int amount);
	int stackItemTab(std::string id, int amount);
	int stackItemInventory(std::string id, int amount);
	int incrementItem(Item* current_item, int amount);
	int decrementItem(std::string id_item, int amount);
	bool existItemTabNoFull(std::string item_id);
	bool existItemInventoryNoFull(std::string item_id);
	bool isTabFull();
	bool isInventoryFull();
	int getTabIndex(sf::Vector2f pos);
	int getCraftIndex(sf::Vector2f pos);
	sf::Vector2i getInventoryIndex(sf::Vector2f pos);
	

	Item* moveItemTab(Item* it, int tab_x);
	Item* moveItemInventory(Item* it, int inv_x, int inv_y);
	void deleteItemTab(int x);
	void deleteMouseItem();
	void deleteItemInventory(int x, int y);
	void inventoryClick(float x, float y, std::string key);
	void craftItem(std::string id_item);
	int getTotalAmountInventory(std::string id_item);
	int getTotalAmountTab(std::string id_item);
	std::string getIdCraftItem(int index);
	
	float x_inventory;
	float y_inventory;
	float x_tab;
	float y_tab;
	float x_craft_list;
	float y_craft_list;


	Item* craft_list[N_CRAFT_ITEMS];
	Item* inventory[Y_SLOTS][X_SLOTS] = { {nullptr} };
	Item* tab[TAB_SLOTS] = { nullptr };
	Item* mouseItem = nullptr;

	TextureManager* texMan;

};