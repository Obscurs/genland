
class Item
{
public:	
	Item(std::string id_item);
	~Item();

	void Load();
	void Reload(std::string filename);

	void SetPosition(float x, float y);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	void SetSize(float x);
	bool IsLoaded() const;
	void decrement(int amount_dec);
	Item* getItemCopy();
	std::map<std::string, int> craft_cost;
	std::string id; //sha de privatitzar
	int amount; //sha de privatitzar
	int max_stack_amount; //sha de privatitzar

	void Draw(sf::RenderWindow& renderWindow);



protected:
	

private:
	sf::Sprite& GetSprite();
	sf::Text text;
	sf::Font font;
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
};
