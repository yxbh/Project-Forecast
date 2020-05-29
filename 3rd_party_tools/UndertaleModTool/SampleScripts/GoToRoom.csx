// Replaces the debug mode "Create system_information_962" option with "Go to room"

EnsureDataLoaded();
ScriptMessage("Add 'Go to room' dialog under F3\nby krzys-h, Kneesnap");

var code = Data.GameObjects.ByName("obj_time").EventHandlerFor(EventType.KeyPress, EventSubtypeKey.vk_f3, Data.Strings, Data.Code, Data.CodeLocals);

Data.Functions.EnsureDefined("get_integer", Data.Strings);
code.ReplaceGML(@"
if (global.debug)
    room_goto(get_integer(""Go to room"", room));
", Data);

ChangeSelection(code);
ScriptMessage("Patched!");