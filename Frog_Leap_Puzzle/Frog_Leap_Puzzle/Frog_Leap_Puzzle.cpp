#include <iostream>
#include <unordered_set>
#include <stack>
#include <set>
#include <vector>
#include <string>
#include <chrono>


// Структура, представляваща състояние на играта
struct State {
    std::string configuration;  // текущата конфигурация на играта
    std::vector<std::string> history;  // история на конфигурациите

    State(const std::string& conf, const std::vector<std::string>& hist ) : configuration( conf ), history( hist ) {}
};

bool isDeadEnd(std::string& current, int size) 
{
    for (unsigned int i = 0; i < size - 3; ++i)
    {
        if (current[i] == '>' && current[i + 1] == '>' &&
            current[i + 2] == '<' && current[i + 3] == '<') 
        {
            return true;
        }
        if ((current[i] == '>' && current[i + 1] == '<' &&
            current[i + 2] == '<' && current[i + 3] == '>') || (
            current[i] == '<' && current[i + 1] == '>' &&
            current[i + 2] == '>' && current[i + 3] == '<'))
        {
            return true;
        }
    }
    return false;
}

// Функция за генериране на всички възможни следващи състояния
void generateNextStates( State currentState, std::vector<State>& nextStates ) {
    
    std::string currentConfig = currentState.configuration;
    int n = currentConfig.size();

    for (int i = 0; i < n; ++i) {
        
        std::string newConfig = currentConfig;

        if (currentConfig[i] == '>' && i < n - 1 && currentConfig[i + 1] == '_') {
            // жабата гледа надясно и има свободно място отпред
            std::swap(newConfig[i], newConfig[i + 1]);   
        }

        if (currentConfig[i] == '<' && i > 0 && currentConfig[i - 1] == '_') {
            // жабата гледа наляво и има свободно място отпред
            std::swap(newConfig[i], newConfig[i - 1]);
        }

        if (i < n - 2 && currentConfig[i] == '>' && currentConfig[i + 2] == '_') {
            // жабата гледа надясно и пред нея има друга жаба
            std::swap(newConfig[i], newConfig[i + 2]);
        }

        if (i > 1 && currentConfig[i] == '<' && currentConfig[i - 2] == '_') {
            // жабата гледа наляво и пред нея има друга жаба
            std::swap(newConfig[i], newConfig[i - 2]);
        }

        if (!isDeadEnd(newConfig, n) && newConfig != currentConfig) {
            std::vector<std::string> newHist = currentState.history;
            newHist.push_back(newConfig);
            State st(newConfig, newHist);
            nextStates.push_back(st);
        }
    }
}

std::string generateFinalConfiguration(int n)
{
    std::string finalConfig;

    for (int i = 0; i < n; ++i) {
        finalConfig += '<';
    }

    finalConfig += '_';

    for (int i = 0; i < n; ++i) {
        finalConfig += '>';
    }

    finalConfig += '\0';

    return finalConfig;
}

// Функция за решаване на задачата с BFS
std::vector<std::string> solveFrogLeapPuzzle(int n, State* outInitState) {
    std::string initialConfig;

    for (int i = 0; i < n; ++i) {
        initialConfig += '>';
    }

    initialConfig += '_';

    for (int i = 0; i < n; ++i) {
        initialConfig += '<';
    }

    initialConfig += '\0';

    std::stack<State> q;
    std::unordered_set<std::string> visited;
    std::vector<std::string> result;

    // Начално състояние
    State initState(initialConfig, {});
    q.push(initState);
    visited.insert(initialConfig);

    std::string finalConfiguration = generateFinalConfiguration(n);

    // DFS
    while (!q.empty()) {
        State currentState = q.top();
        q.pop();

        if (currentState.configuration == finalConfiguration) {
            // Намерили сме крайната конфигурация
            result = currentState.history;
            result.push_back(currentState.configuration);
            break;
        }

        std::vector<State> nextStates;
        generateNextStates(currentState, nextStates);

        for ( const State& nextState : nextStates) {
            if (visited.find(nextState.configuration) == visited.end()) {
                // Ново състояние, което не е посетено
                visited.insert(nextState.configuration);
                q.push(nextState);
            }
        }
    }

    return result;
}


void printResult(const std::vector<std::string>& result) {
    for (const auto& config : result) {
        std::cout << config << std::endl;
    }
}

int main() {
    int n;
    std::cout << "Enter the number of frogs (n): ";
    std::cin >> n;
    State* initState = nullptr;
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> result = solveFrogLeapPuzzle(n, initState);

    auto end = std::chrono::high_resolution_clock::now();

    delete initState;

    std::cout << "Solution steps:" << std::endl;
    printResult(result);

    std::cout << std::endl;
    auto time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << time.count() << " s\n";

    return 0;
}
