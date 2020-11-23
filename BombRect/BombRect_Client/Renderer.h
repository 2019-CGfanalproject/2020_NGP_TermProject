#pragma once
class ObjectContainer;

class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void Initailize(HINSTANCE hInst, HWND hWnd);
	void TestRender();
	void Render(const ObjectContainer& objects);
	void DrawBitmap(BitmapKey key, Vector2 pos);

	HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

private:
	HINSTANCE				m_Instance;
	HWND					m_Wnd;
	ID2D1Factory			* m_Factory;
	ID2D1HwndRenderTarget	* m_RenderTarget;
	IWICImagingFactory		* m_WICFactory;		// 이미지를 불러오기 위함

	std::map<BitmapKey, ID2D1Bitmap*> m_Bitmaps;

	// for test
	ID2D1SolidColorBrush	* m_TestBrush = nullptr;
	ID2D1Bitmap				* m_TestBitmap = nullptr;
	ID2D1Bitmap				* m_TestResourceBitmap = nullptr;
};
