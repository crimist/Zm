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
	// Message
	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 40.f), ImColor(255, 0, 0, 255), "Zm By Syc0x00 Enabled", 0, 0.0f, 0);
	// Dbg info
	ImGuiIO &io = ImGui::GetIO();
	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 55.f), ImColor(255, 0, 0, 255), Helpers::VariableText("%.fx%.f @ %.f fps %.f ms", io.DisplaySize.x, io.DisplaySize.y, io.Framerate, io.DeltaTime * 1000.f), 0, 0.0f, 0);
	// Perf info in microseconds
	float FPSimpact = ((float(this->timeDuration) / 1000.f) * 60.f);
	ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), 20.f, ImVec2(19.f, 70.f), ImColor(255, 0, 0, 255), Helpers::VariableText("Impact: %lld mcs %.0f fps", this->timeDuration, FPSimpact));

	// Set vars
	struct Offsets::gentity_t *local = reinterpret_cast<Offsets::gentity_t *>(OFFSET_GENTITY_PLAYER);

	if (local->Health < 1) // If we have less than 1 hp ret
		return;

	if (Menu::GetInstance()->oCrosshair) {
		float *ADS = reinterpret_cast<float *>(OFFSET_ADS);
		ImVec2 middle = ImVec2(Offsets::Screen->Width / 2, Offsets::Screen->Height / 2);

		ImGui::GetWindowDrawList()->AddCircle(middle, 21 * !(*ADS), ImColor(0, 0, 255, 255), 20, 1);
		ImGui::GetWindowDrawList()->AddCircle(middle, 14 * !(*ADS), ImColor(255, 0, 0, 255), 20, 1);
		ImGui::GetWindowDrawList()->AddCircle(middle, 7 * !(*ADS), ImColor(0, 255, 0, 255), 20, 1);
	}

	if (Menu::GetInstance()->oHealth) {
		int *HealthPtr = reinterpret_cast<int *>(OFFSET_HEALTH);
		int *MaxHealthPtr = reinterpret_cast<int *>(OFFSET_MAXHEALTH);

		// Gradient color change green->yellow->red (Needs testing)
        float healthSmall = fabsf(((float)*HealthPtr / (float)*MaxHealthPtr) - 1);
        ImU32 color = Helpers::ColorGradient(healthSmall);

        /*
		float HealthPercent = ((float)*HealthPtr / (float)*MaxHealthPtr) * 100.f;
		ImU32 color;
        if (HealthPercent > 75.f)
                color = ImColor(0, 255, 0, 255); // 75-100 = Green
        else if (HealthPercent > 50.f)
                color = ImColor(255, 255, 0, 255); // 50-74 = Yellow
        else if (HealthPercent > 25.f)
                color = ImColor(255, 0, 0, 255); // 25-49 = Yellow
        else
                color = ImColor(255, 255, 255, 255); // 0-24 = White
        */

        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(round(Offsets::Screen->Width * .008), round(Offsets::Screen->Height * .744)),
            ImVec2(round(Offsets::Screen->Width * .104), round(Offsets::Screen->Height * .8)),
            ImColor(255, 0, 0, 255), 10);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(round(Offsets::Screen->Width * .008), round(Offsets::Screen->Height * .744)),
            ImVec2(2 * HealthPercent, round(Offsets::Screen->Height * .8)), color, 10);
	}

	float *viewMatrix = reinterpret_cast<float *>(OFFSET_VIEWMATRIX); // This is a float[16]
	float lowestDist = FLT_MAX;
	float *zoom = reinterpret_cast<float *>(OFFSET_ZOOM);
	float x1, x2, y1, y2;
	uint16_t lowestID = UINT16_MAX;
	bool ADS = ((GetKeyState(VK_RBUTTON) & 0x100) != 0);

	if (Menu::GetInstance()->oESP || Menu::GetInstance()->oAimbot || Menu::GetInstance()->oSnap) {
		for (int i = 1; i < 1024; i++) {
			Offsets::gentity_t *ent = GetGentity(i);
			if (ent->Health > 0 && ent->ClientNum < 1000 && ent->Type == EntityType::ZOMBIE) {
				Vector3 pos(ent->Position[0], ent->Position[1], ent->Position[2]);
				float distance = Math::VecDistance(local->Position, ent->Position);

				if (distance < lowestDist) { // Check if it's the closest zombie
					lowestDist = distance;
					lowestID = i;
				}

				if (Menu::GetInstance()->oESP || Menu::GetInstance()->oSnap) {
					Vector3 screen;
					if (Math::WorldToScreen(pos, screen, viewMatrix)) {
						// Box
						x1 = screen.x - (20 / (distance * (0.0025f / *zoom)));
						y1 = screen.y - (100 / (distance * (0.0025f / *zoom)));
						x2 = screen.x + (20 / (distance * (0.0025f / *zoom)));
						y2 = screen.y; // Botom of rect doesn't need to change

						if (Menu::GetInstance()->oESP) // ESP
							ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(255, 0, 0, 255));
						if (Menu::GetInstance()->oSnap) // Snaplines
							ImGui::GetWindowDrawList()->AddLine(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y), ImVec2(screen.x, screen.y), ImColor(255, 0, 0, 255), 1.f);
						// Dbg Info
						//ImGui::GetWindowDrawList()->AddText(ImVec2(screen.x, screen.y - (100 / (distance * (0.0025f / *zoom)))), ImColor(255, 0, 0, 255), Helpers::VariableText("%d %.2f %.0f/%.0f=%.0f", ent->ClientNum, distance, (float)ent->Health, maxHP, HealthPercent));

						// Needs testing
						int *round = reinterpret_cast<int *>(OFFSET_ROUND);
						float maxHP = (150.f + (100.f * (*round - 1)));
						float HealthPercent = ((float)ent->Health / maxHP) * 100.f;
						float healthSmall = fabsf(((float)ent->Health / maxHP) - 1.f);
						ImU32 color = Helpers::ColorGradient(healthSmall);

						/*
						int *round = reinterpret_cast<int *>(OFFSET_ROUND);
						float maxHP = (150.f + (100.f * (*round - 1)));
						float HealthPercent = ((float)ent->Health / maxHP) * 100.f;
						ImU32 color
						if (HealthPercent > 75.f)
							color = ImColor(0, 255, 0, 255); // Green
						else if (HealthPercent > 50.f)
							color = ImColor(255, 255, 0, 255); // Yellow
						else if (HealthPercent > 25.f)
							color = ImColor(255, 0, 0, 255); // Red
						else
							color = ImColor(255, 255, 255, 255); // White
						*/
					
						// Annoying math here
						if (Menu::GetInstance()->oESPHealth) {
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
						}

						if (Menu::GetInstance()->oTrigger) {
							int width = Offsets::Screen->Width / 2;
							int height = Offsets::Screen->Height / 2;

							// If the cursor is inside the zombies ESP box
							if (width >= x1 && width <= x2 && height >= y1 && height <= y2) {
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
			// Bus
			else if (ent->ModelIndex == 44) { // Its at 518
				Vector3 screen;
				Vector3 pos(ent->Position[0], ent->Position[1], ent->Position[2]);

				float distance = VecDistance(local->Position, ent->Position);

				if (Math::WorldToScreen(pos, screen, viewMatrix)) {
					// Box
					x1 = screen.x - (20 / (distance * (0.0025f / *zoom)));
					y1 = screen.y; // top middle of bus
					x2 = screen.x + (20 / (distance * (0.0025f / *zoom)));
					y2 = screen.y + (200 / (distance * (0.0025f / *zoom)));
					ImGui::GetWindowDrawList()->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(255, 0, 0, 255));
					// Snaplines
                    ImGui::GetWindowDrawList()->AddLine(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y), ImVec2(screen.x, screen.y), ImColor(255, 0, 0, 255), 1.f);
                    // Dbg
					ImGui::GetWindowDrawList()->AddText(ImVec2(screen.x, screen.y - (100 / (distance * (0.0025f / *zoom)))), ImColor(255, 0, 0, 255), Helpers::VariableText("Bus %f", distance));
				}
			}
		}
	}

	if (Menu::GetInstance()->oAimbot) {
		if (ADS && lowestID != UINT16_MAX) { // If you're ADSed and closest isn't self
			Offsets::gentity_t *closest = GetGentity(lowestID);
			Vector3 screen, pos(closest->Position[0], closest->Position[1], closest->Position[2]);

			float distance = Math::VecDistance(local->Position, closest->Position);
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
