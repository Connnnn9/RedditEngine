#pragma once

namespace KREngine 
{
    class AppState 
    {
    public:
        virtual ~AppState() = default;

        virtual void Initialize() = 0;
        virtual void Terminate() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
    };
}
