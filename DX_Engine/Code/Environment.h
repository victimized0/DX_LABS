#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

class Environment {
private:
	Environment()
		: m_width(800)
		, m_height(600)
		, m_isFullScreen(false)
		, m_allowFullScreen(false)
		, m_hInst(nullptr)
		, m_hWnd(nullptr)
	{}

	Environment(Environment const&)				= delete;
	Environment& operator=(Environment const&)	= delete;
	Environment(Environment&&)					= delete;
	Environment& operator=(Environment&&)		= delete;

public:
	static Environment& Instance() {
		static Environment instance;
		return instance;
	}

	int GetWidth()const {
		return m_width;
	}

	int GetHeight()const {
		return m_height;
	}

	HINSTANCE GetInstanceHandle() {
		return m_hInst;
	}

	HWND GetWindowHandle() {
		return m_hWnd;
	}

	std::wstring GetExecPath() { return m_execPath; }

	bool GetIsFullscreen() {
		return m_isFullScreen;
	}

	bool GetAllowFullscreen() {
		return m_allowFullScreen;
	}

	void SetWidth(int width) {
		m_width = width;
	}

	void SetHeight(int height) {
		m_height = height;
	}

	void SetInstanceHandle(HINSTANCE hInstance) {
		m_hInst = hInstance;
	}

	void SetWindowHandle(HWND hWnd) {
		m_hWnd = hWnd;
	}

	void SetIsFullscreen(bool isFull) {
		m_isFullScreen = isFull;
	}

	void SetAllowFullscreen(bool allow) {
		m_allowFullScreen = allow;
	}

	void SetExecPath(std::wstring path) { m_execPath = path; }

private:
	std::wstring	m_execPath;

	HINSTANCE		m_hInst;
	HWND			m_hWnd;

	int				m_width;
	int				m_height;

	bool			m_isFullScreen;
	bool			m_allowFullScreen;

};

#endif //_ENVIRONMENT_H_
