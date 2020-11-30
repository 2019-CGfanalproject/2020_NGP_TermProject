#include "pch.h"
#include "Renderer.h"
#include "ObjectContainer.h"
#include "GameObject.h"

BitmapPair g_bitmapInfo[] = {
	{ BitmapKey::BACKGOURND,		L"Assets/default_map.png"		},
	{ BitmapKey::CLOSED_TILE,		L"Assets/closed_tile.png"		},
	{ BitmapKey::CHARACTER_CYAN,	L"Assets/character_cyan.png"	},
	{ BitmapKey::CHARACTER_GREEN,	L"Assets/character_green.png"	},
	{ BitmapKey::CHARACTER_RED,		L"Assets/character_red.png"		},
	{ BitmapKey::CHARACTER_YELLOW,	L"Assets/character_yellow.png"	},
	{ BitmapKey::TITLE,				L"Assets/title.png"				},
	{ BitmapKey::IPADDR_EDITBAR,	L"Assets/ipaddr_editbar.png"	},
	{ BitmapKey::READY_BUTTON,		L"Assets/ready_button.png"		},
	{ BitmapKey::BOMB,				L"Assets/bomb.png"				},
	{ BitmapKey::BOMB_WHITE,		L"Assets/bomb_white.png"		},
	{ BitmapKey::EXPLOSION,			L"Assets/explosion.png"			},

	{ BitmapKey::UI_HEART,			L"Assets/ui_heart.png"			},
	{ BitmapKey::UI_RED,			L"Assets/ui_red.png"			},
	{ BitmapKey::UI_YELLOW,			L"Assets/ui_yellow.png"			},
	{ BitmapKey::UI_GREEN,			L"Assets/ui_green.png"			},
	{ BitmapKey::UI_CYAN,			L"Assets/ui_cyan.png"			},
};


Renderer::Renderer():
	m_Wnd(NULL),
	m_Factory(NULL),
	m_RenderTarget(NULL)
{ 
	// Initialize COM �̶�µ� �� �ʿ����� ��Ȯ�� �𸣰���
	// �̰� �� �ϸ� IWICImagingFactory ����� �� ��������
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

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_WICFactory)
	);

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_WriteFactory)
	);

	m_WriteFactory->CreateTextFormat(
		L"Gabriola",                // Font family name.
		NULL,                       // Font collection (NULL sets it to use the system font collection).
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&m_TextFormat
	);

	LoadGameBitmap();

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

	// for test
	m_RenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightSlateGray),
		&m_TestBrush
	);
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

	D2D1_RECT_F layoutRect = D2D1::RectF(
		0, 0, 200, 100
	);

	m_RenderTarget->DrawTextW(
		L"Hello World",
		11,
		m_TextFormat,
		layoutRect,
		m_TestBrush
	);
	
	m_RenderTarget->EndDraw();
}

void Renderer::DrawLobbyLayout()
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));

	m_RenderTarget->DrawLine(
		D2D1_POINT_2F{ 300.f, 0.f },
		D2D1_POINT_2F{ 300.f, 800.f },
		m_TestBrush
	);

	m_RenderTarget->DrawLine(
		D2D1_POINT_2F{ 300.f, 600.f },
		D2D1_POINT_2F{ 800.f, 600.f },
		m_TestBrush
	);

	m_RenderTarget->EndDraw();
}

void Renderer::Render(const ObjectContainer& objects)
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));

	
	for (auto& object : objects.m_StaticObjects) {
		// ���⼭ ������ �������� �̿��� �������Ѵ�!
		DrawBitmap(object.GetBitmapKey(), object.GetPos());
	}

	for (auto& object : objects.m_DynamicObjects) {
		Vector2 pos{ object.GetPos() };
		pos.x += 40;
		pos.y += 40;
		DrawBitmap(object.GetBitmapKey(), object.GetPos());
	}

	// �۾��� ������ ��� �����̳ʸ� ���� �� �����ͼ� for�� ������

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
			pos.x + size.width, pos.y + size.height )
	);
}

HRESULT Renderer::LoadGameBitmap()
{
	for (auto& info : g_bitmapInfo) {
		ID2D1Bitmap* i_bitmap;

		LoadBitmapFromFile(
			m_RenderTarget,
			m_WICFactory,
			info.file_name.c_str(),
			800, 800,
			&i_bitmap
		);

		m_Bitmaps.emplace(info.key, i_bitmap);
	}

	return S_OK;
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

	// ��Ʈ�� ���ڴ��� �����.
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
		// Direcr2D�� �˾Ƹ��� �� �ִ� �������� ��ȯ�Ѵ�.
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}

	if (SUCCEEDED(hr))
	{
		// FormatConverter�� �ʱ�ȭ�Ѵ�.
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


// ���ӿ� ���Ǵ� �ڿ��� ���ҽ��� �߰���Ű��
// 1. ���� ������ �� ���Ǵ� �̹��� ������ ���� ���� �ʿ� ����.
// 2. ����ڰ� �̹����� �����ϰų� �Ѽ��� �� ����.
// ��� �̷� ���� ��������...?

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
	// ��·�� �� �� �ڵ���

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
