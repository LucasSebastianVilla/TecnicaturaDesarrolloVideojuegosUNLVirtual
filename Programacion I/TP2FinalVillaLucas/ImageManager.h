#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_
class ImageManager {
private:
	ImageManager(const ImageManager&);
	std::map< std::string, sf::Texture > images_;
	std::vector< std::string > resource_directories_;
public:
	ImageManager();
	~ImageManager();
	const sf::Texture& getImage(const std::string& filename);
	void deleteImage(const sf::Texture& image);
	void deleteImage(const std::string& filename);
	void addResourceDirectory(const std::string& directory);
	void removeResourceDirectory(const std::string& directory);
};
#endif
