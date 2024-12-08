#pragma once

namespace KREngine {

    class AppState {
    public:
        virtual ~AppState() = default;

        virtual void Initialize() = 0; // Called when the state is entered
        virtual void Terminate() = 0;  // Called when the state is exited
        virtual void Update() = 0;     // Update logic for the state
        virtual void Render() = 0;     // Render logic for the state
    };
}
