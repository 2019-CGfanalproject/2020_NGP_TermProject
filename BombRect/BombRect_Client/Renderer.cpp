#include "pch.h"
#include "Renderer.h"

Renderer::Renderer():
	m_Wnd(NULL),
	m_Factory(NULL),
	m_RenderTarget(NULL)
{ 
	// Initialize COM 이라는데 왜 필요한지 정확히 모르겠음
	CoInitialize(NULL);		// wincodec.h
}

Renderer::~Renderer()
{
	CoUninitialize();
	SafeRelease(&m_Factory);
	SafeRelease(&m_RenderTarget);
}

void Renderer::Initailize(HWND hWnd)
{
	m_Wnd = hWnd;


	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Factory);

	RECT rc;
	GetClientRect(m_Wnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	m_Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_Wnd, size),
		&m_RenderTarget
	);

	// for test
	m_RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		&m_TestBrush
	);

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_WICFactory)
	);

	LoadBitmapFromFile(
		m_RenderTarget,
		m_WICFactory,
		L"test.bmp",
		100, 100,
		&m_TestBitmap
	);
}

void Renderer::Render()
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	D2D1_SIZE_F rtSize = m_RenderTarget->GetSize();

	// test
	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	for (int x = 0; x < width; x += 10) {
		m_RenderTarget->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
			m_TestBrush,
			1.f
		);
	}
	for (int y = 0; y < height; y += 10)
	{
		m_RenderTarget->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
			m_TestBrush,
			0.5f
		);
	}

	// Retrieve the size of the bitmap.
	D2D1_SIZE_F size = m_TestBitmap->GetSize();

	// Draw a bitmap.
	m_RenderTarget->DrawBitmap(
		m_TestBitmap,
		D2D1::RectF(150, 150, 150 + 200, 150 + 200)
	);
	
	m_RenderTarget->EndDraw();
}

HRESULT Renderer::LoadBitmapFromFile(
	ID2D1RenderTarget* pRenderTarget, 
	IWICImagingFactory* pIWICFactory, 
	PCWSTR uri, 
	UINT destinationWidth, 
	UINT destinationHeight, 
	ID2D1Bitmap** ppBitmap
)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	// 비트맵 디코더를 만든다.
	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Direcr2D가 알아먹을 수 있는 포멧으로 변환한다.
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}

	if (SUCCEEDED(hr))
	{
		// FormatConverter를 초기화한다.
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

HRESULT Renderer::LoadResourceBitmap(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	return S_OK;
}
