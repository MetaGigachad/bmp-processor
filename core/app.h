#pragma once

class App {
public:
    App(int argc, const char** argv) : argc_(argc), argv_(argv) {}

    void Run() const;

private:
    int argc_;
    const char** argv_;
};
