#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES]; //bien ktra vi tri cua player/branch

int main()
{
	VideoMode vm(960, 540); //tao 1 obj window
	RenderWindow window(vm, "Timber");

	View view(sf::FloatRect(0, 0, 1920, 1080)); //tao 1 window moi //960, 540 = 1/4 anh backgrnd
	window.setView(view);

	Texture textureBackground; //tao moi 1 texture
	Texture textureLogo;

	textureBackground.loadFromFile("graphics/background.png"); //load graphic vao texture
	textureLogo.loadFromFile("graphics/happy-tree.png"); //logo

	Sprite spriteBackground; // chen anh vao txtBackgrnd
	Sprite spriteLogo;// them logo

	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);
	spriteLogo.setTexture(textureLogo);
	spriteLogo.setPosition(0,0);

	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0); // make a tree sprite
	
	Text messageText;
	Text scoreText;
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//message text
	messageText.setFont(font);
	messageText.setString("Press Enter to start!");
	messageText.setCharacterSize(60);
	messageText.setColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds(); // tao khung text
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	//ve 2 dam may
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150);

	bool cloud1Active = false;
	bool cloud2Active = false; //may co active ko?

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f; //khoi tao speed mac dinh

	Clock clock; // bien thoi gian
	
	// thanh thoi gian 
	RectangleShape timeBar; 
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2.0f) - timeBarStartWidth / 2.0, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSec = timeBarStartWidth / timeRemaining; // sau 1s thanh Time rut ngan tu 2 dau
	
	bool paused = true; // bien ktra game dang pause ko
	int score = 0;

	//score text
	scoreText.setFont(font);
	scoreText.setString("Score = 0");
	scoreText.setCharacterSize(75);
	scoreText.setColor(Color::White);
	scoreText.setPosition(1400, 20);

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png"); //tao canh cay
	for (int i = 0; i < NUM_BRANCHES; i++) {	//set texture cho branch
		branches[i].setTexture(textureBranch);	//dat vao dead center
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = side::LEFT; // khoi tao 1 player

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(6000, 8600); //tao bia mo khi chet

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830); //tao 1 cai riu

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;	//dat riu o gan goc cay

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720); // khuc go bay sau khi chat
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false; //dieu khien player input

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer); //chop sound

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer); //death sound 

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outofTime;
	outofTime.setBuffer(ootBuffer); //het gio sound

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;	// listen for key press
				
				// ko hien thi axe
				spriteAxe.setPosition(2000,	spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;
			score = 0;
			timeRemaining = 5;

			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}//khoi tao new scene

			spriteRIP.setPosition(675, 2000);
			spritePlayer.setPosition(580, 720);
			acceptInput = true;		//set player vao vi tri
		}// dieu khien player input

		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;

				timeRemaining += (2 / score) + 0.15; // them thoi gian

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720); //cap nhat vi tri player axe
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false; // khuc go bay ra khi chop
				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;

				timeRemaining += (2 / score) + 0.15; // them thoi gian

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720); //cap nhat vi tri player axe
				
				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false; // khuc go bay ra khi chop
				chop.play();
			}
		}
		if (!paused) {
			Time dt = clock.restart(); //bien deltatime
			timeRemaining -= dt.asSeconds(); //tru time moi giay
			timeBar.setSize(Vector2f(timeBarWidthPerSec * timeRemaining, timeBarHeight)); //update size timebar

			if (timeRemaining <= 0.0f) {
				paused = true;
				messageText.setString("Out of time !!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				outofTime.play();
			} //end if timeremain

			//dieu khien may
			if (!cloud1Active) // cloud 1
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200); //toc do cua dam may

				srand((int)time(0) * 10); // do cao
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y); //sau 1s cloud 1 chay voi speed theo truc x

				// Neu may cham den het ria ngoai ben phai thi set no inactive
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false; // chay het may thi tao moi
				}
			}

			if (!cloud2Active) // Cloud 2
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}		//tuong tu cloud1
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str()); //update diem cho player
			
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					branches[i].setPosition(600, height);	//dat canh cay sang trai
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);			// quay sprite 180do vi anh png chia ben phai
				}
				else if (branchPositions[i] == side::RIGHT) {
					branches[i].setPosition(1330, height); //dat canh cay sang phai
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);			// ko quay
				}
				else {
					branches[i].setPosition(3000, height);//hide
				}

				if (logActive) { //dieu khien khuc go 
					spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()*0.1),
										spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()*0.1));
					if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
						logActive = false;
						spriteLog.setPosition(810, 720);
					}
				}

				if (branchPositions[5] == playerSide) {
					paused = true;
					acceptInput = false; // chet

					spriteRIP.setPosition(525, 760);
					spritePlayer.setPosition(2000, 660);
					messageText.setString("SQUISHED!!!");
					FloatRect textRect = messageText.getLocalBounds();
					messageText.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f); // hien thi message 

					messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
					death.play(); //play sound
				}

			}//end if for numbranches
		} //end if(!paused)
				
		// ve scene va hien thi
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteLogo);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2); //ve may

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}	

		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteRIP);
		window.draw(spriteLog);
		window.draw(scoreText);
		window.draw(timeBar);//draw game object
		if (paused) {
			window.draw(messageText);
		}
		window.display();
	}
	return 0;
}

void updateBranches(int seed) {
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1]; //keo tat ca branch xuong 1 cho
	}
	srand((int)time(0) + seed); //sinh canh cay o position 0
	int r = (rand() % 5);
	
	switch (r) //LEFT RIGHT NONE
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}