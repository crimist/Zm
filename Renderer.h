#pragma once

class Renderer {
public:
	void BeginScene();
	void DrawScene();
	void EndScene();

	static Renderer *GetInstance();
private:
	Renderer();
	~Renderer();

	static Renderer *Instance;

	// Performance
	long long timeDuration;
	std::chrono::high_resolution_clock::time_point timeStart;
	std::chrono::high_resolution_clock::time_point timeEnd;
};
