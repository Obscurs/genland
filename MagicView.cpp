//
// Created by pinki.
//

#include "MagicView.h"
#include <iostream>

MagicView::MagicView() : _mode(ViewMode::expanded), _align(ViewAlign::center) {
    _target = nullptr;
    _viewport= {0,0,1,1};
    _offset = {0,0};
}

MagicView::MagicView(sf::RenderTarget* target, ViewMode mode, sf::Vector2i resolution, sf::FloatRect viewport, ViewAlign align)
        : _target(target),
          _mode(mode),
          _align(align),
          _resolution(resolution),
          _viewport(viewport)
{
    update();
    _offset = {0,0};
}


void MagicView::init(sf::Vector2i targetResolution) {
    if (_target == nullptr) { std::cout <<"Trying to init a view that not have a RenderTarget" << std::endl;}
    int windowX = _target->getSize().x, windowY = _target->getSize().y;

    float xr = windowX / float(targetResolution.x);
    float yr = windowY / float(targetResolution.y);
    float aux;
    if (xr < yr) aux = 1 / yr;
    else aux = 1 / xr;

    xr *= aux;
    yr *= aux;

    sf::Vector2d start,size;
    start.x = (1. - yr) / 2.;
    start.y = (1. - xr) / 2.;
    if (_align & (top|down)) {
        start.y = (1. - xr) * ((_align & down) != 0);
    }
    if (_align & (left|right)) {
        start.x = (1. - yr) *((_align & right) != 0);
    }

    size.x = 1. - (1. - yr);
    size.y = 1. - (1. - xr);

    reset(sf::FloatRect(0,0,targetResolution.x,targetResolution.y));
    setViewport(sf::FloatRect(start.x,start.y,size.x,size.y));
    sf::View::move(_offset);
}


void MagicView::initExpanded(sf::Vector2i minResolution) {
    if (_target == nullptr) {std::cout <<"Trying to init a view that not have a RenderTarget"<< std::endl;}
    int windowX = _target->getSize().x, windowY = _target->getSize().y;

    sf::Vector2f ratio = sf::Vector2f(
            float(minResolution.x)/windowX,
            float(minResolution.y)/windowY
    );

    if (ratio.x < ratio.y) {
        ratio = sf::Vector2f(ratio.y/ratio.x, 1);
    }
    else {
        ratio = sf::Vector2f(1, ratio.x/ratio.y);
    }

    setSize(minResolution.x*ratio.x, minResolution.y*ratio.y);
    setCenter(minResolution.x/2.f,minResolution.y/2.f);
    setViewport(sf::FloatRect(0,0,1,1));

    if ((_align & (top|down)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move(0,(+((_align&top)!=0) - ((_align&down)!=0)) * offset);
    }
    if ((_align & (left|right)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move((+((_align&left)!=0) - ((_align&right)!=0)) * offset,0);
    }

    sf::View::move(_offset);
}

void MagicView::initExpanded(sf::Vector2i minResolution, sf::FloatRect viewport) {
    if (_target == nullptr) {std::cout <<"Trying to init a view that not have a RenderTarget"<< std::endl;}
    int windowX = _target->getSize().x, windowY = _target->getSize().y;


    sf::Vector2f ratio = sf::Vector2f(
            float(minResolution.x)/windowX,
            float(minResolution.y)/windowY
    );

    if (ratio.x < ratio.y) {
        ratio = sf::Vector2f(ratio.y/ratio.x, 1);
    }
    else {
        ratio = sf::Vector2f(1, ratio.x/ratio.y);
    }

    sf::Vector2f size(minResolution.x*ratio.x, minResolution.y*ratio.y);

    size.x /= viewport.height;
    size.y /= viewport.width;

    setSize(size);
    setCenter(minResolution.x/2.f,minResolution.y/2.f);
    setViewport(viewport);

    if ((_align & (top|down)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move(0,(+((_align&top)!=0) - ((_align&down)!=0)) * offset);
    }
    if ((_align & (left|right)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move((+((_align&left)!=0) - ((_align&right)!=0)) * offset,0);
    }
    sf::View::move(_offset);
}

void MagicView::initExpandedFit(sf::Vector2i minResolution, sf::FloatRect viewport) {
    if (_target == nullptr) {std::cout <<"Trying to init a view that not have a RenderTarget"<< std::endl;}
    int windowX = _target->getSize().x, windowY = _target->getSize().y;

    windowX *= viewport.width;
    windowY *= viewport.height;

    minResolution.x *= viewport.width;
    minResolution.y *= viewport.height;

    sf::Vector2f ratio = sf::Vector2f(
            float(minResolution.x)/windowX,
            float(minResolution.y)/windowY
    );

    if (ratio.x < ratio.y) {
        ratio = sf::Vector2f(ratio.y/ratio.x, 1);
    }
    else {
        ratio = sf::Vector2f(1, ratio.x/ratio.y);
    }

    sf::Vector2f size(minResolution.x*ratio.x, minResolution.y*ratio.y);

    setSize(size);
    setCenter(minResolution.x/2.f,minResolution.y/2.f);
    setViewport(viewport);

    if ((_align & (top|down)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move(0,(+((_align&top)!=0) - ((_align&down)!=0)) * offset);
    }
    if ((_align & (left|right)) != 0) {
        float offset = getSize().y/2 - getCenter().y;
        sf::View::move((+((_align&left)!=0) - ((_align&right)!=0)) * offset,0);
    }
    sf::View::move(_offset);
}

void MagicView::initHardCodedSizeHorizontal(float width, sf::FloatRect viewport) {
    if (_target == nullptr) {std::cout <<"Trying to init a view that not have a RenderTarget"<< std::endl;}

    float height = (width/viewport.width) * (float(_target->getSize().y)/_target->getSize().x);
    // float height = width;
    setSize(width, height);
    setViewport(viewport);
    setCenter(width/2, height/2);
}

void MagicView::update() {
    switch(_mode) {
        case ViewMode::crop:
            init(_resolution);
            break;
        case ViewMode::expanded:
            initExpanded(_resolution);
            break;
        case ViewMode::viewport:
            initExpanded(_resolution, _viewport);
            break;
        case ViewMode::viewportConvervativeSize:
            initExpandedFit(_resolution, _viewport);
            break;
        case ViewMode::viewportHardcodedWidth:
            initHardCodedSizeHorizontal(_resolution.x, _viewport);
            break;
    }
}

void MagicView::move(float offsetX, float offsetY) {
    _offset += {offsetX, offsetY};
    sf::View::move(offsetX, offsetY);
}

void MagicView::zoomToMouse(float factor) {
    _resolution = sf::Vector2i(sf::Vector2f(_resolution) * factor);
    const sf::Vector2f pos = sf::Vector2f(_target->mapPixelToCoords(sf::Mouse::getPosition(*static_cast<sf::RenderWindow*>(_target)),*this));
    const sf::Vector2f pixel(_target->mapCoordsToPixel(pos, *this));
    const sf::Vector2f beforeCoord(_target->mapPixelToCoords(sf::Vector2i(pixel), *this));
    sf::View::zoom(factor);
    const sf::Vector2f afterCoord(_target->mapPixelToCoords(sf::Vector2i(pixel), *this));
    const sf::Vector2f offsetCoords(beforeCoord - afterCoord);
    sf::View::move(offsetCoords);
}

void MagicView::setRenderTarget(sf::RenderTarget* target) {
    _target = target;
}

void MagicView::setMode(MagicView::ViewMode mode) {
    _mode = mode;
}

void MagicView::setResolution(sf::Vector2i resolution) {
    _resolution = resolution;
    update();
}

void MagicView::setOffset(sf::Vector2f offset) {
    _offset = offset;
    update();
}


sf::Vector2i MagicView::getResolution() const {
    return _resolution;
}

sf::Vector2f MagicView::getMouseCoord() const {
    return sf::Vector2f(_target->mapPixelToCoords(sf::Mouse::getPosition(*static_cast<sf::RenderWindow*>(_target)),*this));
}

sf::Vector2i MagicView::getPixel(sf::Vector2f coord) const {
    return _target->mapCoordsToPixel(coord, *this);
}


sf::Vector2f MagicView::getMapCoord(sf::Vector2i pixel) const {
    return sf::Vector2f(_target->mapPixelToCoords(pixel,*this));
}

sf::FloatRect MagicView::getWindowRect() const {
    return sf::FloatRect(getCenter().x - getSize().x/2, getCenter().y - getSize().y/2, getSize().x, getSize().y);
}