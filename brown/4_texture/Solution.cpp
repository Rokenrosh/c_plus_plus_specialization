#include "Common.h"
#include <iostream>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Geometry: public IShape{
public:
    void SetPosition(Point pos) override{
        pos_ = pos;
    }

    Point GetPosition() const override{
        return pos_;
    }

    void SetSize(Size size) override{
        size_ = size;
    }
    Size GetSize() const override{
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> texture) override{
        texture_ = texture;
    }

    ITexture* GetTexture() const override{
        return texture_.get();
    }
protected:
    Size size_;
    Point pos_;
    shared_ptr<ITexture> texture_;
};

class Rectangle: public Geometry{
    std::unique_ptr<IShape> Clone() const override{
        unique_ptr<IShape> ptr = make_unique<Rectangle>();
        ptr->SetPosition(pos_);
        ptr->SetSize(size_);
        ptr->SetTexture(texture_);
        return ptr;
    }

    void Draw(Image& image) const override{
        Size texture_size = { 0, 0 };
        if (texture_) {
            texture_size = texture_->GetSize();
        }
        for(int i = pos_.y; i < pos_.y + size_.height && i < image.size(); i++){
            for(int j = pos_.x; j < pos_.x + size_.width && j < image[i].size(); j++){
                if (texture_) {
                    if (i - pos_.y < texture_size.height && j - pos_.x < texture_size.width) {
                        image[i][j] = texture_->GetImage()[i - pos_.y][j - pos_.x];
                    }
                    else {
                        image[i][j] = '.';
                    }
                }
                else {
                    image[i][j] = '.';
                }
            }
        }
    }
};

class Ellipse: public Geometry{
    std::unique_ptr<IShape> Clone() const override{
        unique_ptr<IShape> ptr = make_unique<Ellipse>();
        ptr->SetPosition(pos_);
        ptr->SetSize(size_);
        ptr->SetTexture(texture_);
        return ptr;
    }

    void Draw(Image& image) const override{
        Size texture_size = { 0, 0 };
        if (texture_) {
            texture_size = texture_->GetSize();
        }
        for(int i = pos_.y; i < pos_.y + size_.height && i < image.size(); i++){
            for(int j = pos_.x; j < pos_.x + size_.width && j < image[i].size(); j++){
                if(IsPointInEllipse({j - pos_.x, i - pos_.y}, size_)){
                    if (texture_) {
                        if (i - pos_.y < texture_size.height && j - pos_.x < texture_size.width) {
                            image[i][j] = texture_->GetImage()[i - pos_.y][j - pos_.x];
                        }
                        else {
                            image[i][j] = '.';
                        }
                    }
                    else {
                        image[i][j] = '.';
                    }
                }
            }
        }
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if(shape_type == ShapeType::Rectangle){
        return make_unique<Rectangle>();
    }
    if(shape_type == ShapeType::Ellipse){
        return make_unique<Ellipse>();
    }
    return nullptr;
}