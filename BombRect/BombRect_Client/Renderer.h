#pragma once
class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void Initailize(HWND hWnd);
	void Render();

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
	HWND					m_Wnd;
	ID2D1Factory			* m_Factory;
	ID2D1HwndRenderTarget	* m_RenderTarget;
	IWICImagingFactory		* m_WICFactory;		// 이미지를 불러오기 위함

	// for test
	ID2D1SolidColorBrush	* m_TestBrush = nullptr;
	ID2D1Bitmap				* m_TestBitmap = nullptr;
};
