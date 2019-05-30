#include "common.h"
#include <future>
#include <iostream>
#include <sstream>

LPD3DXFONT pFont;

DriverController driver("r5apex.exe");

RECT overlayRect;
RECT gameRect;

void ESP();

int colors[] = { 0xffff0000, 0xffff6600, 0xffffc300, 0xffe5ff00, 0xff8cff00, 0xff19ff00, 0xff00ffb2, 0xff00fffa, 0xff00aeff, 0xff007fff, 0xff002aff, 0xff9000ff, 0xffe100ff, 0xffff00c3, 0xffff0050, 0xff268700, 0xff005c87, 0xff3c0087, 0xff4ebf8e, 0xffff8989, 0xff870024 , 0xff88eaf7, 0xff88f7d7, 0xff80aa00 };

int maxDistance = 300;

Helpers helpers;
std::mutex screenDimMutex;
int ScreenX;
int ScreenY;
int menuHeight = 15;//start at 15 cause for first element
uintptr_t CenterTarget = 0;
void AimbotThread();

std::mutex aimbotmutex;

int GetScreenWidth()
{
	screenDimMutex.lock();
	int val = ScreenX;
	screenDimMutex.unlock();
	return val;
}

int GetScreenHeight()
{
	screenDimMutex.lock();
	int val = ScreenY;
	screenDimMutex.unlock();
	return val;
}

Vec3 GetVelocity(uintptr_t ent)
{
	return driver.rpm<Vec3>(ent + helpers.Entity_Velocity);
}

#define BoneX_Offset 0xCC
#define BoneY_Offset 0xDC
#define BoneZ_Offset 0xEC
#define BonesOffset 0xED8

Vec3 GetBone(uint64_t Entity, int32_t Bone, Vec3 Position)
{
	Vec3 TargetBone;

	uintptr_t Bones = driver.rpm<uintptr_t>(Entity + BonesOffset);

	TargetBone.x = driver.rpm<float>(Bones + BoneX_Offset + Bone * 0x30) + Position.x;
	TargetBone.y = driver.rpm<float>(Bones + BoneY_Offset + Bone * 0x30) + Position.y;
	TargetBone.z = driver.rpm<float>(Bones + BoneZ_Offset + Bone * 0x30) + Position.z;

	return TargetBone;
}

bool writeteam = false;
int main()
{
	Sleep(100);
	helpers.modbase = driver.GetModuleBase("r5apex.exe");

	std::thread ESPDataThread(ESP);
	std::thread AimBotThread(AimbotThread);
	int team = 0;
	
	

	while (1)
	{
		if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
		{
			std::cout << driver.rpm<float>(helpers.modbase + 0x18E3D50) << std::endl;
			driver.wpm<float>(1.0f, helpers.modbase + 0x18E3D50);
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		{
			maxDistance += 100;
			std::cout << maxDistance << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		{
			maxDistance -= 100;
			std::cout << maxDistance << std::endl;

		}

		if (GetAsyncKeyState(VK_HOME) & 0x8000)
		{
			writeteam = !writeteam;
			if (writeteam)
			{
				team = driver.rpm<int>(helpers.GetLocalEntity() + helpers.Entity_Team);
				std::cout << "Team swap on: " << team << std::endl;
			}
			
			else
			{
				std::cout << "Team swap off" << std::endl;
				driver.wpm<int>((team), helpers.GetLocalEntity() + helpers.Entity_Team);
			}


		}

		if (writeteam)
		{
			driver.wpm<int>(100, helpers.GetLocalEntity() + helpers.Entity_Team);
		}
		
		Sleep(100);
	}

	return 0;
}

#define DEG2RAD( x  )  ( (float)(x) * (float)(3.14159265358979323846f / 180.f) )

static void SinCos(float radians, float* sine, float* cosine)
{
	*sine = (float)sin(radians);
	*cosine = (float)cos(radians);
}

void AngleVectors(const Vec3& angles, Vec3* forward)
{

	float	sp, sy, cp, cy;
	
	SinCos(DEG2RAD(angles.x), &sy, &cy);
	SinCos(DEG2RAD(angles.y), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

const char* font = "Univers";
bool aimbot = true;

void AimbotThread()
{
	float smoothing = 3.0f;
	
	while (1)
	{
		if (GetAsyncKeyState(VK_NUMPAD8) & 1)
		{
			smoothing += 1.0f;
			system("cls");
			std::cout << smoothing << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			smoothing -= 1.0f;


			if (smoothing <= 0.0f)
			{
				smoothing = 0.0f;
			}

			system("cls");
			std::cout << smoothing << std::endl;
		}

		if (GetAsyncKeyState('F'))
		{
			//aimbotmutex.lock();
			if (CenterTarget)
			{
				uintptr_t localEnt = helpers.GetLocalEntity();
				Vec3 Velocity = GetVelocity(CenterTarget); //Target entity velocity.

				Vec3 tmp = driver.rpm<Vec3>(CenterTarget + helpers.Position_Feet); //Getting feet position.
				Vec3 closestCoords = GetBone(CenterTarget, 10, tmp); //Getting neck bone position.
				//Vec3 localVel = GetVelocity(localEnt);
				Vec3 Dynamic = driver.rpm<Vec3>(localEnt + 0x20a8);//Delete if breath not working

	
				//Vec3 curview = driver.rpm<Vec3>(localEnt + 0x20b8);//Delete if breath not working
				//closestCoords += Dynamic;
				//Vec3 breath = Dynamic - curview; //Delete if breath not working
				Vec3 camPos = driver.rpm<Vec3>(localEnt + helpers.Position_Head); //Getting local head position
				float distance = Distance(closestCoords, camPos) / 0.01905f; //Distance from local to target

				float BulletSpeed = helpers.GetBulletSpeed(); //bullet speed

				float HorizontalTime = distance / BulletSpeed;
				float VerticalTime = distance / BulletSpeed;


				closestCoords.x += (Velocity.x * HorizontalTime);
				closestCoords.y += (Velocity.y * HorizontalTime);
				closestCoords.z += (750.f * (VerticalTime * VerticalTime));
				//closestCoords.z += (750.f * VerticalTime * VerticalTime);

				Vec3 delta = closestCoords - camPos;
				Vec3 localangles = driver.rpm<Vec3>(localEnt + 0x20B8);

				Vec3 breath;
				AngleVectors(Dynamic, &breath);

				Vec3 angleout;
				VectorAngles(delta, angleout);

				Vec3 recoil = driver.rpm<Vec3>(localEnt + 0x2014);
				//angleout -= breath; //Delete if breath not working
				//Vec3 breath = dynamic - localangles;
				//angleout -= breath;
				angleout -= recoil;
				
				//angleout += Dynamic;
				angleout.z = 0.f;

				Vec3 aimstep = angleout - localangles;
				aimstep = ClampAngle(aimstep);
				
				aimstep.x /= smoothing;
				aimstep.y /= smoothing;



				//std::cout << aimstep.x << " " << aimstep.y << std::endl;

				localangles += aimstep;


				Vec3 target = ClampAngle(localangles);
				driver.wpm<Vec3>(target, localEnt + 0x20B8);
			}
			Sleep(1);
		}
		
		else
		{
			Sleep(10);
		}
	}
}

HANDLE hThread;

void ESP()
{
	std::string strWindowTitle("", 0x40);

	strWindowTitle = "Apex Legends";

	Overlay::IOverlay *pOverlay = new Overlay::CD3D9Overlay();
	pOverlay->GetSurface()->PrepareFont(font, font, 13, FW_NORMAL, 0, 0, FALSE);
	if (pOverlay->Create(strWindowTitle))
	{
		auto pSurfaceFontData = pOverlay->GetSurface()->GetSurfaceFontData(font);
		pFont = (LPD3DXFONT)pSurfaceFontData->m_pSurfaceFont;

		
		auto EspCallback = [](Overlay::IOverlay *pOverlay, std::shared_ptr< Overlay::ISurface > pSurface)
		{
			try
			{	
				pOverlay->ScaleOverlayWindow();
				screenDimMutex.lock();
				ScreenX = pOverlay->GetWidth();
				ScreenY = pOverlay->GetHeight();
				screenDimMutex.unlock();

				
				D3DXMATRIX matrix = helpers.GetViewMatrix();
				uintptr_t localEnt = helpers.GetLocalEntity();

				Vec3 LocalPos = driver.rpm<Vec3>(localEnt + helpers.Position_Head);
				Vec3 centerScreen((GetScreenWidth() / 2), (GetScreenHeight() / 2), 0.0f);
				if (GetAsyncKeyState(VK_INSERT) & 1)
				{
					aimbot = !aimbot;

					if (aimbot)
					{
						std::cout << "aimbot on" << std::endl;
					}
					else
					{
						std::cout << "aimbot off" << std::endl;
					}
				}
				int closestToCenter = 99999;

				int localTeam = driver.rpm<int>(localEnt + helpers.Entity_Team);
				for (size_t i = 0; i < 62; i++)
				{
					
					uintptr_t entity = driver.rpm<uintptr_t>(helpers.modbase + helpers.gEntityList + (i * 0x20));
					if (entity == localEnt)
					{
						continue;
					}
					if (entity)
					{
						int enemyteam = driver.rpm<int>(entity + helpers.Entity_Team);

						if (localTeam == enemyteam)
						{
							continue;
						}

						Vec3 headpos = driver.rpm<Vec3>(entity + helpers.Position_Head);
						int dist = Distance(LocalPos, headpos);


						if ((dist) <= maxDistance)
						{
							Vec3 headout;
							Vec3 feetout;
							if (world_to_screen(matrix, headpos, headout))
							{
								if (headout.x <= 1920 && headout.x >= 0 && headout.y <= 1080 && headout.y >= 0)
								{
									int health = driver.rpm<int>(entity + helpers.Entity_Health);
									int sheild = driver.rpm<int>(entity + helpers.Entity_Sheild);

									if (health <= 0)
									{
										continue;
									}
									std::stringstream all;
									all << dist << "m" << " " << health + sheild << "hp";

									Vec3 feetpos = driver.rpm<Vec3>(entity + helpers.Position_Feet);
									world_to_screen(matrix, feetpos, feetout);
									int height = Distance(headout, feetout) / 0.01905f;

									pSurface->BorderBox(headout.x - height/4, headout.y, height/2, height, 2, colors[enemyteam]);

									pSurface->String(feetout.x, feetout.y, font, colors[enemyteam], all.str().c_str());
	

									if (aimbot)
									{
										if (!GetAsyncKeyState('F'))
										{
											int distToCenter = Distance(centerScreen, headout) / 0.01905f;

											if (distToCenter < closestToCenter)
											{
												closestToCenter = distToCenter;
												CenterTarget = entity;

											}


											if (closestToCenter > 200)
											{
												CenterTarget = 0;
											}
										}									
									}									
								}
							}
						}
					}
				}

			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		};

		pOverlay->AddToCallback(EspCallback);

		while (pOverlay->Render())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}

	pOverlay->Shutdown();
	delete pOverlay;
	system("pause");
}