class EnemyShip : public Ship
{
	public :
		EnemyShip();
		int armadaSize();
	private :
		int health;
		int numberOfEnemies;
};

class Grunt : protected EnemyShip
{
	public:
		Grunt();
	private:
		int health;
};