#include "pch.h"
#include "Renderer.h"
#include "ObjectContainer.h"
#include "GameObject.h"

Renderer::Renderer():
	m_Wnd(NULL),
	m_Factory(NULL),
	m_RenderTarget(NULL)
{ 
	// Initialize COM 이라는데 왜 필요한지 정확히 모르겠음
	// 이거 안 하면 IWICImagingFactory 사용할 때 에러난다
	CoInitialize(NULL);		// wincodec.h
}

Renderer::~Renderer()
{
	CoUninitialize();
	SafeRelease(&m_Factory);
	SafeRelease(&m_RenderTarget);
}

void Renderer::Initailize(HINSTANCE hInst, HWND hWnd)
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

	//LoadBitmapFromFile(
	//	m_RenderTarget,
	//	m_WICFactory,
	//	L"test.bmp",
	//	100, 100,
	//	&m_TestBitmap
	//);

	ID2D1Bitmap* tmp;

	LoadBitmapFromFile(
		m_RenderTarget,
		m_WICFactory,
		L"default_map.png",
		800, 800,
		&tmp
	);

	m_Bitmaps.emplace(BitmapKey::BACKGOURND, tmp);

	LoadBitmapFromFile(
		m_RenderTarget,
		m_WICFactory,
		L"closed_tile.png",
		800, 800,
		&tmp
	);

	m_Bitmaps.emplace(BitmapKey::CLOSED_TILE, tmp);

	// Resource Test
	/*
	LoadResourceBitmap(
		m_RenderTarget,
		m_WICFactory,
		L"IDB_BITMAP1",
		L"BITMAP",
		200, 0,
		&m_TestResourceBitmap
	);

	LoadResourceBitmap(
		m_RenderTarget,
		m_WICFactory,
		L"SampleImage",
		L"Image",
		200, 0,
		&m_TestResourceBitmap
	);
	*/
}

void Renderer::TestRender()
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

	// Resource Test
	/*
	size = m_TestResourceBitmap->GetSize();

	m_RenderTarget->DrawBitmap(
		m_TestResourceBitmap,
		D2D1::RectF(0, 0, size.width, size.height)
	);
	*/
	
	m_RenderTarget->EndDraw();
}

void Renderer::Render(const ObjectContainer& objects)
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	
	for (auto& object : objects.m_StaticObjects) {
		// 여기서 가져온 정보들을 이용해 렌더링한다!
		DrawBitmap(object.GetBitmapKey(), object.GetPos());
	}

	m_RenderTarget->EndDraw();
}

void Renderer::DrawBitmap(BitmapKey key, Vector2 pos)
{
	auto bitmap = m_Bitmaps.find(key);
	if (m_Bitmaps.end() == bitmap) return;

	D2D1_SIZE_F size = (bitmap->second)->GetSize();

	m_RenderTarget->DrawBitmap(
		bitmap->second,
		D2D1::RectF(pos.x, pos.y,
			pos.x + size.width, pos.y + size.height)
	);
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


// 게임에 사용되는 자원을 리소스로 추가시키면
// 1. 앱을 배포할 때 사용되는 이미지 파일을 따로 보낼 필요 없다.
// 2. 사용자가 이미지를 삭제하거나 훼손할 수 없다.
// 어떻게 이런 일이 가능한지...?

HRESULT Renderer::LoadResourceBitmap(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory, 
	PCWSTR resourceName, 
	PCWSTR resourceType,
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

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// Locate the resource.
	// typedef HANDLE HINSTANCE
	// typedef HANDLE HMODULE
	// 어쨌던 둘 다 핸들임

	imageResHandle = FindResourceW(NULL, resourceName, resourceType);
	HRESULT hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(NULL, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(NULL, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;

	}

	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = pIWICFactory->CreateStream(&pStream);
	}
	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = pIWICFactory->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);

		if (SUCCEEDED(hr))
		{
			//create a Direct2D bitmap from the WIC bitmap.
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
}
