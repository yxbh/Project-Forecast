#pragma once

#include <QString>

#include <functional>
#include <memory>


namespace ke
{

    using ProgressAnnouncer = std::function<void(const QString&)>;

    class IAssetsProcessor
    {
    public:
        IAssetsProcessor(ProgressAnnouncer progressAnnouncer) : progressAnnouncer(progressAnnouncer) {}
        virtual ~IAssetsProcessor() = 0;

        virtual bool process() = 0;

    protected:
        inline void updateProgress(const QString & msg) { progressAnnouncer(msg); }

    protected:
        ProgressAnnouncer progressAnnouncer;
    };

    inline IAssetsProcessor::~IAssetsProcessor() {}
    inline bool IAssetsProcessor::process() { return false; }

    class NullAssetsProcessor : public IAssetsProcessor
    {
    public:
        using IAssetsProcessor::IAssetsProcessor;

        virtual bool process() final;

    };

    inline bool NullAssetsProcessor::process()
    {
        return false;
    }

    using AssetsProcessorSptr = std::shared_ptr<IAssetsProcessor>;

}
