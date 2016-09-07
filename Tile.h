

class Tile
{
public:
	Tile(int id_temp);
	~Tile();
	
	void Load(std::map<std::string, sf::Texture> &texture_map);
	void Reload(std::string id, std::map<std::string, sf::Texture> &texture_map);
	//void Remove();
	void Draw(sf::RenderWindow & window, int layer);
	void Update(float elapsedTime);

	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool colisionable; //sha de privatitzar
	bool visible; //sha de privatitzar
	bool rigid;
	bool reach_floor;
	int max_tension;
	int weight;
	bool IsLoaded() const;
	sf::Rect<float> GetBoundingRect() const;
	std::string id; //sha de privatitzar
    int id_temp;
	bool lights[8];


	

private:


	sf::Sprite& GetSprite();
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
};

