#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}



bool Scene::CheckPath(char* path)
{
	if (!FileExists(path)) { return false; }
	return true;
}

bool Scene::CheckPath(const char* path)
{
	if (!FileExists(path)) { return false; }
	return true;
}

bool Scene::CheckPath(WCHAR* path)
{
	if (!FileExists(path)) { return false; }
	return true;
}

bool Scene::CheckAllPaths(HWND hwnd)
{
	std::wstring msg = L"Some files weren't found:\n";
	std::wstring ws;
	bool all_correct = true;
	bool result = true;


	for (char* c : m_cpPaths)
	{
		ws = m_converter.from_bytes(c) + L"\n";
		result = CheckPath(c);
		all_correct &= result;
		if (!result) msg += ws;
	}

	for (WCHAR* w : m_wcpPaths)
	{
		ws = w + std::wstring(L"\n");
		result = CheckPath(w);
		all_correct &= result;
		if (!result) msg += ws;
	}



	// CHANGE TO every shader/model checking their own stuff ¿??¿
	if (!all_correct)
	{
		MessageBox(hwnd, msg.c_str(), L"Error", MB_OK);
		return false;
	}
	else
	{
		m_cpPaths.clear();
		m_wcpPaths.clear();
	}

	return all_correct;
}




float Scene::DistanceToCamera(GameObject* go)
{
	XMVECTOR goPos = go->GetWorldMatrix().r[3];
	XMVECTOR caPos = XMLoadFloat3(&(m_Camera->GetPosition()));
	
	return XMVectorGetX(XMVector3LengthSq(goPos - caPos));
}


float Scene::GetFOV()
{
	return m_Camera->GetFOV();
}
