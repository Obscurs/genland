//
// Created by pinki.
//

#ifndef TORRATEST_MAGICVIEW_H
#define TORRATEST_MAGICVIEW_H


#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

namespace sf {
    typedef sf::Vector2<double> Vector2d;
}

class MagicView : public sf::View {
public:

    enum ViewMode {
        crop2,
        crop, // If the aspectratio of the window is not the same of the viewresolution, the viewport adapts.
        expanded, // If the aspectratio of the window is not the same of the viewresolution, the resolution adapts.
        viewport, // Same as 'expanded' but saying what viewport do you want.
        viewportConvervativeSize, //
        viewportHardcodedWidth // Width hardcoded, good ratio width/height, need a viewport, top centered
    };

    enum ViewAlign {
        top = 1,
        down = top << 1,
        left = down << 1,
        right = left << 1,
        center = right << 1
    };

    MagicView();
    MagicView(sf::RenderTarget* target, ViewMode mode, sf::Vector2i resolution, sf::FloatRect viewport = {0,0,1,1}, ViewAlign align = center);


    void init(sf::Vector2i targetResolution);
    void init2(sf::Vector2i targetResolution);
    void initExpanded(sf::Vector2i minResolution);
    void initExpanded(sf::Vector2i targetResolution, sf::FloatRect viewport);
    void initExpandedFit(sf::Vector2i targetResolution, sf::FloatRect viewport);
    void initHardCodedSizeHorizontal(float width, sf::FloatRect viewport);


    void update();

    void move(float offsetX, float offsetY);
    void zoomToMouse(float factor);

    void setRenderTarget(sf::RenderTarget* target);
    void setMode(ViewMode mode);
    void setResolution(sf::Vector2i resolution);
    void setOffset(sf::Vector2f offset);

    sf::Vector2i getResolution() const;
    sf::Vector2f getMouseCoord() const;
    sf::Vector2i getPixel(sf::Vector2f coord) const;
    sf::Vector2f getMapCoord(sf::Vector2i pixel) const;
    sf::FloatRect getWindowRect() const;

private:
    sf::RenderTarget* _target;
    ViewMode _mode;
    ViewAlign _align;
    sf::Vector2i _resolution;
    sf::Rect<float> _viewport;
    sf::Vector2f _offset;
};

#endif
