class EnemyShip : public Ship
{
	public :
		EnemyShip();
		~EnemyShip();
		int getArmadaSize();
		void increaseArmada();
		void decreaseArmada();
		EnemyShip *nextShip;
		EnemyShip *prevShip;
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