#include <vector>

class Inputs
{
public:
	Inputs();
	virtual ~Inputs();

	virtual void Update();
	sf::Vector2i getKey(std::string s);

protected:

	float col_right_dist;

private:
	void UpdateKey(bool pressed, std::string key);
	std::map<std::string, sf::Vector2i> keys;
};