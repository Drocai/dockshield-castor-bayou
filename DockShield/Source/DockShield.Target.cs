using UnrealBuildTool;
using System.Collections.Generic;

public class DockShieldTarget : TargetRules
{
    public DockShieldTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("DockShield");
    }
}
