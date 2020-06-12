#include "KEngine/Systems/ImguiSystem.hpp"

#if defined(USE_SFML)
#include "KEngine/Events/SFML/SfmlEvent.hpp"
#endif

#include "KEngine/Graphics/DearImgui/ImguiSceneNodes.hpp"
#include "KEngine/Entity/Components/EntityRenderableComponents.hpp"
#include "KEngine/Events/AppConsoleEvents.hpp"
#include "KEngine/Core/EventManager.hpp"
#include "KEngine/Core/Keyboard.hpp"
#include "KEngine/App.hpp"
#include "KEngine/Log/Log.hpp"
#include "KEngine/Utility/StringHelper.hpp"

#include "KEngine/Graphics/Imgui.hpp"

#if defined(USE_SFML)
#include <SFML/Graphics/RenderWindow.hpp>
#include "imgui-SFML.h"
#endif

#include <array>
#include <vector>

namespace ke::priv
{
    static auto logger = ke::Log::createDefaultLogger("AppConsoleSystem");

    class AppConsoleSystem : public ke::ISystem
    {
        KE_DEFINE_SYSTEM_COMMON_PROPERTIES(AppConsoleSystem, 0xE97C2771)
        
    public:
        virtual bool initialise(void) final;
        virtual void shutdown(void) final;

        virtual void update(ke::Time elapsedTime) final;

        void handleEvent(ke::EventSptr event);

    private:
        void setupConsoleIfUninitialised(void);
        void drawConsole(void);
        int consoleInputEditCallback(ImGuiInputTextCallbackData* data);
        void processConsoleCommand(const char * const command);

        void addConsoleLog(const char* fmt, ...) IM_FMTARGS(2);
        void clearConsoleLog(void);


        ke::EntityWptr consoleEntity;

        bool consoleInitialised = false;
        bool isConsoleOpen = true;
        bool isConsoleAutoScroll = true;
        bool isConsoleScrollToBottom = true;

        using ConsoleHistoryContainer = std::vector<std::string>;

        std::array<char, 512>       consoleInputBuf = {};
        ImGuiTextFilter             consoleFilter;
        std::vector<std::string>    consoleItems;
        std::vector<std::string>    consoleCommands;
        ConsoleHistoryContainer     consoleHistory;
        ConsoleHistoryContainer::size_type  consoleHistoryPos = std::numeric_limits<ConsoleHistoryContainer::size_type>::max();  // max: new line, 0..History.Size-1 browsing history.
    };

    bool AppConsoleSystem::initialise(void)
    {
        ke::EventManager::registerListener<ke::RequestShowAppConsoleEvent>(this, &AppConsoleSystem::handleEvent);
        ke::EventManager::registerListener<ke::RequestHideAppConsoleEvent>(this, &AppConsoleSystem::handleEvent);
        ke::EventManager::registerListener<ke::RequestToggleAppConsoleDisplayEvent>(this, &AppConsoleSystem::handleEvent);
        return true;
    }

    void AppConsoleSystem::shutdown(void)
    {
        auto consoleEntitySptr = this->consoleEntity.lock();
        if (consoleEntitySptr)
        {
            auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
            entityManager->removeEntity(consoleEntitySptr->getId());
        }

        ke::EventManager::deregisterListener<ke::RequestShowAppConsoleEvent>(this, &AppConsoleSystem::handleEvent);
        ke::EventManager::deregisterListener<ke::RequestHideAppConsoleEvent>(this, &AppConsoleSystem::handleEvent);
        ke::EventManager::deregisterListener<ke::RequestToggleAppConsoleDisplayEvent>(this, &AppConsoleSystem::handleEvent);
    }

    void AppConsoleSystem::update(ke::Time elapsedTime)
    {
        KE_UNUSED(elapsedTime);
        this->setupConsoleIfUninitialised();
    }

    void AppConsoleSystem::handleEvent(ke::EventSptr event)
    {
        KE_UNUSED(event);
        switch (event->getType())
        {
        case ke::RequestShowAppConsoleEvent::TYPE:
            this->isConsoleOpen = true;
            break;
        case ke::RequestHideAppConsoleEvent::TYPE:
            this->isConsoleOpen = false;
            break;
        case ke::RequestToggleAppConsoleDisplayEvent::TYPE:
            this->isConsoleOpen = !this->isConsoleOpen;
            break;
        default:
            logger->warn("AppConsoleSystem: Unhandled event: {}", event->getName());
            break;
        }
    }

    void AppConsoleSystem::setupConsoleIfUninitialised(void)
    {
        if (this->consoleInitialised) return;

        auto entityManager = ke::App::instance()->getLogic()->getEntityManager();
        auto newEntity = entityManager->newEntity();
        auto entity = newEntity.lock();

        auto node = ke::ImguiNode::create(entity->getId());
        auto imguiNode = std::dynamic_pointer_cast<ke::ImguiNode>(node);
        ke::graphics::ImguiRenderInfo imguiInfo;
        imguiInfo.imguiDrawer = [this]()
        {
            if (this->isConsoleOpen)
            {
                this->drawConsole();
            }
        };
        imguiNode->states.info = imguiInfo;

        auto renderableComponent = ke::makeEntityComponent<ke::EntityRenderableComponent>(entity);
        renderableComponent->setSceneNode(imguiNode);
        entity->addComponent(renderableComponent);

        this->consoleEntity = entity;

        this->consoleInitialised = true;
    }

    void AppConsoleSystem::drawConsole(void)
    {
        ImGui::SetNextWindowSize(ImVec2(540, 600), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("KENGINE CONSOLE", &this->isConsoleOpen))
        {
            logger->debug("!ImGui::Begin(\"KENGINE CONSOLE\", &this->isConsoleOpen)");
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                this->isConsoleOpen = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        if (ImGui::SmallButton("Add Dummy Text"))
        {
            this->addConsoleLog("%d some text", this->consoleItems.size());
            this->addConsoleLog("some more text");
            this->addConsoleLog("display very important message here!");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Add Dummy Error"))
        {
            this->addConsoleLog("[error] something went wrong");
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("CLEAR"))
        {
            this->clearConsoleLog();
        }
        ImGui::SameLine();
        bool copyToClipboard = ImGui::SmallButton("COPY");
        ImGui::SameLine();
        if (ImGui::SmallButton("SCROLL TO BOTTOM"))
        {
            this->isConsoleScrollToBottom = true;
        }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("OPTIONS"))
        {
            if (ImGui::Checkbox("Auto-scroll", &this->isConsoleAutoScroll) && this->isConsoleAutoScroll)
            {
                this->isConsoleScrollToBottom = true;
            }
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("OPTIONS"))
        {
            ImGui::OpenPopup("OPTIONS");
        }
        ImGui::SameLine();
        this->consoleFilter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("CLEAR"))
            {
                this->clearConsoleLog();
            }
            ImGui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
        // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
        // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
        //     ImGuiListClipper clipper(Items.Size);
        //     while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copyToClipboard)
        {
            ImGui::LogToClipboard();
        }
        for (const auto & consoleItem : this->consoleItems)
        {
            if (!this->consoleFilter.PassFilter(consoleItem.data()))
                continue;

            // Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
            bool pop_color = false;
            if (consoleItem.find("[error]") != std::string::npos)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                pop_color = true;
            }
            else if (strncmp(consoleItem.c_str(), "> ", 2) == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
                pop_color = true;
            }
            ImGui::TextUnformatted(consoleItem.c_str());
            if (pop_color)
                ImGui::PopStyleColor();
        }
        if (copyToClipboard)
        {
            ImGui::LogFinish();
        }
        if (this->isConsoleScrollToBottom)
        {
            ImGui::SetScrollHereY(1.0f);
            this->isConsoleScrollToBottom = false;
        }
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaimFocus = false;
        const auto inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        auto inputEditCallback = [](ImGuiTextEditCallbackData* data)
        {
            auto consoleSystem = reinterpret_cast<AppConsoleSystem*>(data->UserData);
            return consoleSystem->consoleInputEditCallback(data);
        };
        ImGui::AlignTextToFramePadding(); ImGui::Text("INPUT"); ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::InputText("", this->consoleInputBuf.data(), this->consoleInputBuf.size(), inputTextFlags, inputEditCallback, this))
        {
            if (this->consoleInputBuf[0])
            {
                this->processConsoleCommand(this->consoleInputBuf.data());
            }
            this->consoleInputBuf[0] = '\0';
            reclaimFocus = true;
        }
        ImGui::PopItemWidth();

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaimFocus)
        {
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }

        ImGui::End();
    }

    int AppConsoleSystem::consoleInputEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';' || c == ':')
                    break;
                --word_start;
            }
            const std::string word { word_start, word_end };

            // Build a list of candidates
            std::vector<std::string> candidates;
            if (!word.empty())
            {
                candidates.reserve(this->consoleCommands.size());
                for (const auto & consoleCommand : this->consoleCommands)
                {
                    const std::string wordUpper = ke::upper_copy(word);
                    const std::string cmdUpper  = ke::upper_copy(consoleCommand);
                    if (std::strncmp(cmdUpper.data(), wordUpper.data(), wordUpper.length()) == 0)
                    {
                        candidates.push_back(consoleCommand);
                    }
                }
            }

            if (!word.empty() &&candidates.size() == 0)
            {
                // No match
                this->addConsoleLog("No match for \"%s\"!\n", word.c_str());
            }
            else if (candidates.size() == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0].c_str());
                data->InsertChars(data->CursorPos, " ");
            }
            else if (candidates.size() > 1)
            {
                // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.size() && all_candidates_matches; i++)
                        if (i == 0)
                            c = std::toupper(candidates[i][match_len]);
                        else if (c == 0 || c != std::toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0].data(), candidates[0].data() + match_len);
                }

                // List matches
                this->addConsoleLog("Possible matches:\n");
                for (const auto & candidate : candidates)
                {
                    this->addConsoleLog("- %s\n", candidate.c_str());
                }
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const auto prevHistoryPos = this->consoleHistoryPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (this->consoleHistoryPos == std::numeric_limits<ConsoleHistoryContainer::size_type>::max())
                    this->consoleHistoryPos = this->consoleHistory.size() - 1;
                else if (this->consoleHistoryPos > 0)
                    this->consoleHistoryPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (this->consoleHistoryPos != std::numeric_limits<ConsoleHistoryContainer::size_type>::max())
                    if (++this->consoleHistoryPos >= this->consoleHistory.size())
                        this->consoleHistoryPos--;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prevHistoryPos != this->consoleHistoryPos)
            {
                auto historyItem = (this->consoleHistoryPos >= 0) ? this->consoleHistory[this->consoleHistoryPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, historyItem.data());
            }
        }
        }
        return 0;
    }

    void AppConsoleSystem::processConsoleCommand(const char * const commandBuf)
    {
        std::string command{ commandBuf };
        ke::trim(command);
        this->addConsoleLog(("> " + command  + "\n").c_str());
        if (this->consoleHistory.empty() || (!this->consoleHistory.empty() && this->consoleHistory[this->consoleHistory.size() - 1] != command))
        {
            this->consoleHistory.emplace_back(command);
        }
    }

    void AppConsoleSystem::addConsoleLog(const char* fmt, ...)
    {
        // FIXME-OPT
        char buf[2048];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        this->consoleItems.emplace_back(buf);
        if (this->isConsoleAutoScroll)
        {
            this->isConsoleScrollToBottom = true;
        }
    }

    void AppConsoleSystem::clearConsoleLog(void)
    {
        this->consoleItems.clear();
        this->isConsoleScrollToBottom = true;
    }

} // ns ke::priv


namespace ke
{
    AppConsoleSystem::AppConsoleSystem(void)
    {
        this->systemImpl = std::make_unique<ke::priv::AppConsoleSystem>();
    }

    bool AppConsoleSystem::initialise(void)
    {
        return this->systemImpl->initialise();
    }

    void AppConsoleSystem::shutdown(void)
    {
        this->systemImpl->shutdown();
    }

    void AppConsoleSystem::update(ke::Time elapsedTime)
    {
        this->systemImpl->update(elapsedTime);
    }

} // namespace ke