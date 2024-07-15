#include "TextureHolder.h"
// include the "assert feature"
#include <assert.h>
TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	// Get a reference to m_Textures m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	// auto is the equivalent of map<string, texture>
	// Create an interator to hold a key-value-pair (kvp)
	// and search for the required kvp
	// using the passed in file name
	auto keyValuePair = m.find(filename);
	// auto in equivalent of map <string, texture>::iterator

	// Did we find a match?
	if (keyValuePair != m.end())
	{
		// yes
		// Return the future,
		// the second part of the kvp, the texture
		return keyValuePair->second;
	}
	else
	{
		// File name not found
		// Create a new key value pair using the filename
		auto& texture = m[filename];
		// Load the texture from file in the usual way
		texture.loadFromFile(filename);
		// return the texture to the calling code
		return texture;
	}
}