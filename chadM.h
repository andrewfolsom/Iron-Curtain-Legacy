class EnemyShip : public Ship
{
	public :
		EnemyShip();
		~EnemyShip();
		int getArmadaSize();
		void increaseArmada();
		void decreaseArmada();

		//EDITS by JACKSON
		void updateStrafe();
		void updateRush();
		void updateCircle();
		void updateBank();
		void updateDiagRush();
		void updatePosition();
		//


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
