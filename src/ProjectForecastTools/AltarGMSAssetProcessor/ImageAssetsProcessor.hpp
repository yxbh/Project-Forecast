#pragma once

#include "IAssetsProcessor.hpp"
#include "Common.hpp"

#include <QImage>
#include <QList>
#include <QString>
#include <QRgb>

namespace ke
{

    class ImageAssetsProcessor : public IAssetsProcessor
    {
    public:
        static AssetsProcessorSptr createProcessor(
                ProgressAnnouncer progress, const QString & backgroundFolderPath,
                const QString & spriteFolderPath, const QString & texpageFolderPath,
                const QString & textureFolderPath, const QString & outputFolderPath,
                const QRgb & backgroundFillColour, bool overrideAlpha0PixelsWithBgFillColour,
                bool generateGif, std::size_t gifFrameDelayInMs);

        using IAssetsProcessor::IAssetsProcessor;

        virtual bool process() final;

    private:
        void processBackgroundFiles();
        void processSpriteFiles();
        void processTexpageFiles();
        void processTextureFiles();

        bool generateBackgroundFiles();
        bool generateSpriteFiles();
        bool generateGifFiles();

    private:
        QString backgroundFolderPath;
        QString spriteFolderPath;
        QString texpageFolderPath;
        QString textureFolderPath;
        QString processingOutputFolderPath;

        QRgb backgroundFillColour;
        bool isOverrideAlpha0PixelsWithBgFillColour;

        struct Background
        {
            QString name;
            TexpageId texpageId;
        };

        struct Sprite
        {
            Dimension2D dimension;
            BoundingBox2D boundingBox;

            unsigned boundingBoxMode = 0;

            Position origin;

            bool separateMasks = true;

            QString name;

            QList<TexpageId> texpageIds;
        };

        struct Texpage
        {
            Position sourcePosition;
            Dimension2D sourceDimension;

            Position destinationPosition;
            Dimension2D destinationDimension;

            Dimension2D dimension;

            TextureId textureId;
            TexpageId id;
        };

        struct Texture
        {
            QImage texture;

            TextureId id;
        };

        QList<Texpage> texpages;
        QList<Texture> textures;
        QList<Sprite> sprites;
        QList<Background> backgrounds;

        bool isGeneratingGif;
        std::size_t gifFrameDelayInMs;

    };

}


