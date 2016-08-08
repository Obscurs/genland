

class Tile
{
public:
	Tile();
	~Tile();
	
	void Load();
	void Reload(std::string id);
	void Remove();
	void Draw(sf::RenderWindow & window);
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


	

private:
	sf::Sprite& GetSprite();
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
};

