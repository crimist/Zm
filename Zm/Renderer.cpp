#include "stdafx.h"
#include "Renderer.h"
#include "Vectors.h"
#include "Offsets.h"
#include "Helpers.h"
#include "Math.h"
#include "Menu.h"

Renderer* Renderer::Instance;
Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::BeginScene() {
	this->timeStart = std::chrono::high_resolution_clock::now();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("D3D11_Buffer", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
}

void Renderer::DrawScene() {
	// TODO optimize
	// Message
	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 40.f), ImColor(255, 0, 0, 255), "Zm By Syc0x00 Enabled", 0, 0.0f, 0);

	// Dbg info
	ImGuiIO &io = ImGui::GetIO();

	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 55.f), ImColor(255, 0, 0, 255), Helpers::VariableText("%.fx%.f @ %.f fps %.f ms", io.DisplaySize.x, io.DisplaySize.y, io.Framerate, io.DeltaTime * 1000.f), 0, 0.0f, 0);
	// Perf info in microseconds
	float FPSimpact = ((float(this->timeDuration) / 1000.f) * 60.f);
	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 70.f), ImColor(255, 0, 0, 255), Helpers::VariableText("Impact: %lld mcs %.0f fps", this->timeDuration, FPSimpact));

	struct Offsets::gentity_t *local = reinterpret_cast<Offsets::gentity_t *>(OFFSET_GENTITY_PLAYER);
	if (local->Health > 0) {
		if (Menu::GetInstance()->oCrosshair) {
			float *ADS = reinterpret_cast<float *>(0x0103ACA0);
			
			ImVec2 middle = ImVec2(Offsets::Screen->Width / 2, Offsets::Screen->Height / 2);
			ImGui::GetWindowDrawList()->AddCircle(middle, 21 * !(*ADS), ImColor(0, 0, 255, 255), 20, 1);
			ImGui::GetWindowDrawList()->AddCircle(middle, 14 * !(*ADS), ImColor(255, 0, 0, 255), 20, 1);
			ImGui::GetWindowDrawList()->AddCircle(middle, 7 * !(*ADS), ImColor(0, 255, 0, 255), 20, 1);
		}

		if (Menu::GetInstance()->oHealth) {
			int *HealthPtr = reinterpret_cast<int *>(OFFSET_HEALTH);
			int *MaxHealthPtr = reinterpret_cast<int *>(OFFSET_MAXHEALTH);
			float HealthPercent = ((float)*HealthPtr / (float)*MaxHealthPtr) * 100.f;

			ImU32 color;
			if (HealthPercent > 75.f)
				color = ImColor(0, 255, 0, 255); // Green
			else if (HealthPercent > 50.f)
				color = ImColor(255, 255, 0, 255); // Yellow
			else if (HealthPercent > 25.f)
				color = ImColor(255, 0, 0, 255); // Red
			else
				color = ImColor(255, 255, 255, 255); // White

			ImGui::GetWindowDrawList()->AddRect(
				ImVec2(round(Offsets::Screen->Width * .008), round(Offsets::Screen->Height * .744)),
				ImVec2(round(Offsets::Screen->Width * .104), round(Offsets::Screen->Height * .8)),
				ImColor(255, 0, 0, 255),
				10);
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(round(Offsets::Screen->Width * .008), round(Offsets::Screen->Height * .744)),
				ImVec2(2 * HealthPercent, round(Offsets::Screen->Height * .8)),
				color, 
				10);
		}
	}

	float viewMatrix[16];
	int v = 0;
	for (int i = 0; i < 64; i = (i + 0x4)) {
		float *value = reinterpret_cast<float *>(0x033F0400 + i);
		viewMatrix[v++] = *value;
	}

	Offsets::gentity_t *self = GetGentity(0);
	float lowestDist = FLT_MAX;
	int lowestID = 0;
	bool ADS = false;
	if ((GetKeyState(VK_RBUTTON) & 0x100) != 0)
		ADS = true;
	float x1;
	float y1;
	float x2;
	float y2;
	float *zoom = reinterpret_cast<float *>(0x103AD20);

	if (Menu::GetInstance()->oESP || Menu::GetInstance()->oAimbot) {
		for (int i = 2; i < 1024; i++) { // Ignore ourselves
			Offsets::gentity_t *ent = GetGentity(i);
			if (ent->Health > 0 && ent->ClientNum < 1000 && ent->Type == EntityType::ZOMBIE) {
				Vector3 screen;
				Vector3 pos;
				pos.x = ent->Position[0];
				pos.y = ent->Position[1];
				pos.z = ent->Position[2];
				float distance = Math::VecDistance(self->Position, ent->Position);
				if (distance < lowestDist) {
					lowestDist = distance;
					lowestID = i;
				}

				if (Menu::GetInstance()->oESP) {
					if (Math::WorldToScreen(pos, screen, viewMatrix)) {
						// Box
						int *round = reinterpret_cast<int *>(OFFSET_ROUND);
						float maxHP = (150.f + (100.f * (*round - 1)));
						float HealthPercent = ((float)ent->Health / maxHP) * 100.f;
						x1 = screen.x - (20 / (distance * (0.0025f / *zoom)));
						y1 = screen.y - (100 / (distance * (0.0025f / *zoom)));
						x2 = screen.x + (20 / (distance * (0.0025f / *zoom)));
						y2 = screen.y; // Botom of rect doesn't need to change

						// ESP
						ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(255, 0, 0, 255));
						// Snaplines
						ImGui::GetWindowDrawList()->AddLine(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y), ImVec2(screen.x, screen.y), ImColor(255, 0, 0, 255), 1.f);
						// Dbg
						//ImGui::GetWindowDrawList()->AddText(ImVec2(screen.x, screen.y - (100 / (distance * (0.0025f / *zoom)))), ImColor(255, 0, 0, 255), Helpers::VariableText("%d %.2f %.0f/%.0f=%.0f", ent->ClientNum, distance, (float)ent->Health, maxHP, HealthPercent));

						ImU32 color;
						if (HealthPercent > 75.f)
							color = ImColor(0, 255, 0, 255); // Green
						else if (HealthPercent > 50.f)
							color = ImColor(255, 255, 0, 255); // Yellow
						else if (HealthPercent > 25.f)
							color = ImColor(255, 0, 0, 255); // Red
						else
							color = ImColor(255, 255, 255, 255); // White

						// Annoying math here
						// Zombie health outline
						ImGui::GetWindowDrawList()->AddRect(
							ImVec2(screen.x - (15 / (distance * (0.0025f / *zoom))), y2 - (5 / (distance * (0.0025f / *zoom)))),
							ImVec2(screen.x + (15 / (distance * (0.0025f / *zoom))), y2),
							ImColor(255, 0, 0, 255));
						// Zombie Health colored in
						ImGui::GetWindowDrawList()->AddRectFilled(
							ImVec2(screen.x - (15 / (distance * (0.0025f / *zoom))), y2 - (5 / (distance * (0.0025f / *zoom)))),
							ImVec2(screen.x + (-15.f + (HealthPercent / (3.3325f))) / (distance * (0.0025f / *zoom)), y2),
							color);

						if (Menu::GetInstance()->oTrigger) {
							int width = Offsets::Screen->Width / 2;
							int height = Offsets::Screen->Height / 2;

							if (width >= x1 &&
								width <= x2 &&
								height >= y1 &&
								height <= y2) {
								
								static std::chrono::high_resolution_clock::time_point lastShot;
								long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastShot).count();
								if (duration > Menu::GetInstance()->oTriggerDelay) {
									mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
									lastShot = std::chrono::high_resolution_clock::now();
								}
							}
						}
					}
				}
			}
			/*else if (ent->ModelIndex == 44) { // Its at 518
			Vector3 screen;
			Vector3 pos;
			pos.x = ent->Position[0];
			pos.y = ent->Position[1];
			pos.z = ent->Position[2];
			float distance = VecDistance(self->Position, ent->Position);

			if (WorldToScreen(pos, screen, viewMatrix, 1600, 900)) {
			// Box
			x1 = screen.x - (20 / (distance * (0.0025f / *zoom)));
			y1 = screen.y; // top middle of bus
			x2 = screen.x + (20 / (distance * (0.0025f / *zoom)));
			y2 = screen.y + (200 / (distance * (0.0025f / *zoom)));
			ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(255, 0, 0, 255));
			// Snaplines
			ImGui::GetWindowDrawList()->AddLine(ImVec2(1600 / 2, 900), ImVec2(screen.x, screen.y), ImColor(255, 0, 0, 255), 1.f);
			// Dbg
			ImGui::GetWindowDrawList()->AddText(ImVec2(screen.x, screen.y - (100 / (distance * (0.0025f / *zoom)))), ImColor(255, 0, 0, 255), Helpers::VariableText("%d %d %f", ent->ClientNum, ent->ModelIndex, distance));
			}
			}*/
		}
	}

	if (Menu::GetInstance()->oAimbot) {
		if (ADS && lowestID != 0) { // If they're ADS and closest isn't self
			Offsets::gentity_t *zombie = GetGentity(lowestID);
			Vector3 screen;
			Vector3 pos;
			pos.x = zombie->Position[0];
			pos.y = zombie->Position[1];
			pos.z = zombie->Position[2];
			float distance = Math::VecDistance(self->Position, zombie->Position);
			bool onscreen = Math::WorldToScreen(pos, screen, viewMatrix);

			if (Menu::GetInstance()->oUnrealAimbot || onscreen) // if its unreal or if the ent is actually onscreen
				if (!SetCursorPos(screen.x, screen.y - (60 / (distance * (0.0025f / *zoom))))) // 0.0018f / zoom% or 0.0018f * zoom%
					Console::Log("Failed to SetCursorPos()");
		}
	}
}

void Renderer::EndScene() {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DrawList->PushClipRectFullScreen();

	ImGui::End();
	ImGui::PopStyleColor();

	this->timeEnd = std::chrono::high_resolution_clock::now();
	timeDuration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart).count();
}

Renderer *Renderer::GetInstance() {
	if (!Instance)
		Instance = new Renderer();

	return Instance;
}
