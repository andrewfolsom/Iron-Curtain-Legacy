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
		void configStrafe(float radius, float angleSet, float angleSpeed);
		void updateRush();
		void configRush(float speed);
		void updateCircle();
		void configCircle(float radius, float angleSet, float angleSpeed);
		void updateBank();
		void configBank(float destX, float destY, float speed);
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
