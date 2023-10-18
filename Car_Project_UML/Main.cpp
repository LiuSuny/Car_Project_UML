#include <iostream>
#include <conio.h>
#include <thread>
using namespace std;



#define Escape 27
#define Enter 13


#define MIN_TANK_VOLUME	 35 //#define create  macro(global) definition
#define MAX_TANK_VOLUME	160

class Tank
{
	const int VOLUME;	//Character of object
	double fuel_level;	//condition of object
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		if (fuel + this->fuel_level > VOLUME)this->fuel_level = VOLUME;
		else this->fuel_level += fuel;
	}
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume) :VOLUME(
		volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
		volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
		volume),
		fuel_level(0)
	{
		/*
		if (volume < 35)volume = 35;
		if (volume > 160)volume = 160;
		this->VOLUME = volume;
		*/
		cout << "TConstructor:\t" << this << endl;
	}
	~Tank()
	{
		cout << "TDestructor:\t" << this << endl;
	}

	void info()const
	{
		cout << "Volume:\t" << get_VOLUME() << " liters.\n";
		cout << "Fuel:\t" << get_fuel_level() << " liters.\n";
	}
};

#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 30

class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
	double consumptionRate = 0.0;
public:
	double getConsumption()const
	{
		return CONSUMPTION;
	}
	double getConsumption_per_second()const
	{
		return consumption_per_second;
	}
	bool started()const
	{
		return is_started;
	}

	void setConsumptionperSecond()
	{
		consumption_per_second = CONSUMPTION * 3e-5;
	}


	void start()
	{
		is_started = true;
	}

	void stop()
	{
		is_started = false;
	}


	//constructor using tenary method
	Engine(double amount_Of_consumption) :CONSUMPTION(amount_Of_consumption <  MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		amount_Of_consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION : amount_Of_consumption)
	{
		setConsumptionperSecond();
		is_started = false;
		std::cout << "Econstructor \t" << this << std::endl;

	}


	~Engine()
	{
		std::cout << "Edestructor " << this << std::endl;
	}

	void printEngineInfo()const
	{
		std::cout << "Engine consumption for 100km: = " << CONSUMPTION << " Liter" << std::endl;
		std::cout << "Consuption per second : = " << consumption_per_second << " Liter" << std::endl;
		std::cout << "engine move : = " << (is_started ? "start" : "stop") << std::endl;
	}
};

#define MAX_SPEED_LOW_LIMIT 120 
#define MAX_SPEED_HIGH_LIMIT 400
#define MIN_ACCELERATION  10
#define MAX_ACCELERATION  40


class Auto_Car
{
	Engine engine;
	Tank tank;
	const int MAX_SPEED;
	const int ACCELERATION;
	int Speed;
	bool driver_Inside; //when driver inside the car he will car panel initialization
	double _consumption_rate_per_speed = 0.0;

	//To get our car started and moving we need to initiaize struct
	struct
	{
		std::thread panal_thread; //in this threads only the panel will be launched
		std::thread engine_idle_thread;
		std::thread free_Wheeling_thread; //Силь трения
	}threads;//annonimious object inialization allow is to tap to several function outside of private class
	bool busy; //Driver is busy
public:

	Auto_Car(Engine engine, Tank tank, int MaxSpeed = 250, int accelerate = 10) :
		Speed(0),
		tank(tank),
		engine(engine),
		MAX_SPEED
		(
			MaxSpeed< MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			MaxSpeed>MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT : MaxSpeed
		),
		ACCELERATION
		(
			accelerate<MIN_ACCELERATION ? MIN_ACCELERATION :
			accelerate>MAX_ACCELERATION ? MAX_ACCELERATION : accelerate
		)
	{

		driver_Inside = false;// needed if the driver is truily inside
		busy = false;
		std::cout << "Your Car is ready, kindly press Enter to get inside" << std::endl;
	}

	~Auto_Car()
	{
		std::cout << "Car is over" << std::endl;
	}

	void get_in()
	{
		driver_Inside = true;
		threads.panal_thread = std::thread(&Auto_Car::Panel, this);
		//std::thread- creation of object class 'thread'(creation of thread)
		//&Car::panel - pointer to a method (member-function) that will be executed in the thread.
	   //In order to run a method in a thread, you must also indicate for which 
		//object this method will be executed
	}

	void get_out()
	{
		system("CLS");
		driver_Inside = false;
		//meaning if the panel thread is joinable then go ahead and join
		//It is customary to synchronize threads. The join() method synchronizes threads
		if (threads.panal_thread.joinable())threads.panal_thread.join();
		//https://legacy.cplusplus.com/reference/thread/thread/join/
		//https://legacy.cplusplus.com/reference/thread/thread/joinable/
		std::cout << "You are out on the street" << std::endl;
	}
	//with this function our car can start read fuel relevel while moving using thread
	void start()
	{

		engine.start();
		threads.engine_idle_thread = std::thread(&Auto_Car::engine_idle, this);

	}
	//with this function our car can stop reading fuel level is there is no movement
	void stop()
	{
		engine.stop();
		//if this engine idle is joinable then we need to join it
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}
	void accelerate()
	{
		if (driver_Inside && engine.started() && !busy)
		{

			Speed += ACCELERATION;
			if (Speed > MAX_SPEED) Speed = MAX_SPEED;
			std::this_thread::sleep_for(1s);
			//busy = false;
			if (Speed > 0 && !threads.free_Wheeling_thread.joinable())
				threads.free_Wheeling_thread = std::thread(&Auto_Car::free_Wheeling, this);
			busy = true;


		}

	}
	void slow_down()
	{

		if (driver_Inside && !busy)
		{
			Speed -= ACCELERATION;
			if (Speed < 0)Speed = 0;
			std::this_thread::sleep_for(1s);
			//busy = false;
			if (Speed == 0 && threads.free_Wheeling_thread.joinable())
				threads.free_Wheeling_thread.join();
			busy = true;
		}
	}


	void control()
	{
		int key;
		do
		{
			key = 0; //reset the key pressed
			if (_kbhit()) ////The kbhit() function returns 'true' when any key is pressed.
				key = _getch(); ////Waits for a key press and returns the ASCII code of the key pressed
			switch (key)
			{
			case Enter: driver_Inside ? get_out() : get_in();
				/*if (driver_Inside)get_out();
				else get_in();*/ //the same thing as the line above 
				break;
			case 'F': //fill
			case 'f':
				if (driver_Inside)
				{
					std::cout << "First, get out of the car ";
					break;
				}
				double fuel; //initisllizing fuel to get the volume of our fuel
				std::cout << "Kindly enter volume of fuel: ";
				cin >> fuel;
				tank.fill(fuel); //calling our func on class fuel

				break;
			case 'I'://ignition 
			case 'i': if (driver_Inside)engine.started() ? stop() : start();
				//if(driver_Inside)!engine.started() ? start() : stop(); 
				break;
				//w car accelerate when you press
			case 'W':
			case 'w': if (!busy) accelerate();
				break;
				//S car accelerate when you press
			case 'S':
			case 's':
				if (!busy) slow_down();
				break;
			case Escape:
				stop();
				get_out();
				std::cout << "GoodBye" << std::endl;

			}
			//if (Speed < 0)Speed = 0;
			if (tank.get_fuel_level() == 0)stop();//once our fuel read to 0 tge car should automatically stop movement
			if (Speed == 0 && threads.free_Wheeling_thread.joinable())
				threads.free_Wheeling_thread.join();
		} while (key != Escape);
	}
	void  free_Wheeling()
	{
		while (--Speed > 0)
		{
			//Speed--;
			std::this_thread::sleep_for(1s);
		}

		/*while (Speed--)
		{
			if (Speed < 0)Speed = 0;
			{
				engine.setConsumptionperSecond();
				std::this_thread::sleep_for(1s);
			}
		}*/
	}
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.getConsumption_per_second()))
		{
			std::this_thread::sleep_for(1s);
			busy = false;
		}

	}

	void Panel()//panel for car infront of the driver
	{
		while (driver_Inside)
		{
			system("CLS");
			std::cout << "Speed: \t" << Speed << "km\h" << std::endl;
			std::cout << "fuel level :\t" << tank.get_fuel_level() << " Liters" << std::endl;
			if (tank.get_fuel_level() <= 5.0)
			{
				std::cout << "LOW FUEL - Refill your tank!" << std::endl;

			}
			std::cout << "Engine is " << (engine.started() ? "Started" : "Stopped") << std::endl;
			if (Speed >= 1 && Speed <= 60)
			{
				_consumption_rate_per_speed = 0.0020;
			}
			else if (Speed >= 61 && Speed <= 100) {
				_consumption_rate_per_speed = 0.0014;
			}
			else if (Speed >= 101 && Speed <= 140) {
				_consumption_rate_per_speed = 0.0020;
			}
			else if (Speed >= 141 && Speed <= 200) {
				_consumption_rate_per_speed = 0.0025;
			}
			else if (Speed >= 201 && Speed <= 250) {
				_consumption_rate_per_speed = 0.0030;
			}
			else {
				_consumption_rate_per_speed = 0.0; // Car is not moving
			}
			std::cout << "Fuel Consumption per km\h = " << (!engine.started() ? 0 : _consumption_rate_per_speed) << " liter/second " << std::endl;
			if (Speed == 0)
			{
				std::cout << "The car is not moving " << std::endl;
			}
			else if (Speed > 150)
			{
				std::cout << "Kindly slow down, you are speeding too fast " << std::endl;
			}

			std::this_thread::sleep_for(1s);//this function make our console to be blinking to avoid blinking on console, 
		}
	}

	void printCar()const
	{
		engine.printEngineInfo();
		tank.info();
		std::cout << "Max Speed :\t" << MAX_SPEED << "km\h" << std::endl;
		std::cout << "Acceleration :\t" << ACCELERATION << "km\h" << std::endl;
		std::cout << "Speed:\t\t" << Speed << "km\h" << std::endl;
	}


};


//#define TANK_CHECK

//#define ENGINE_TANK

#define CAR_RUNNING

void main()
{
	setlocale(LC_ALL, "Rus");
	srand(time(NULL));


#ifdef TANK_CHECK //ctr+K+S to find #ifdef and type your #def of TANK_CHECK
	Fuel_Tank tank(20);
	tank.printTank();
#endif //TANK_CHECK


#ifdef ENGINE_TANK //ctr+K+S to find #ifdef and type your #def of TANK_CHECK
	Engine engine(20);
	engine.printEngineInfo();
#endif // ENGINE_TANK


#ifdef CAR_RUNNING
	Auto_Car myCar(10, 60);

	//myCar.printCar();
	//myCar.Panel();
	myCar.control();

#endif // CAR_RUNNING


}