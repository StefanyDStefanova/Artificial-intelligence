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

    State() {}
    State(const std::string& conf, const std::vector<std::string>& hist) : configuration(conf), history(hist){}
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
void generateNextStates(State currentState, std::vector<State>& nextStates) {

    std::string currentConfig = currentState.configuration;
    int n = currentConfig.size();

    for (int i = 0; i < n; ++i) {

        std::string newConfig = currentConfig;
        std::string newMove;

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

std::string generateMidConfiguration(int n) 
{
    std::string midConfig;
    for (int i = 0; i < n; ++i) 
    {
        midConfig += '>';
        midConfig += '<';
    }
    midConfig += "_";

    midConfig += '\0';
    return midConfig;
}

void makeMiddleMoves( State& currentState, int n ) 
{
    std::string newConfig = currentState.configuration;
    for (int i = n*2; i >= 2; i-=2) {
        std::swap(newConfig[i], newConfig[i - 2]);
        currentState.history.push_back(newConfig);
    }
}

int findIndexOfEmptyLily(const std::string& config) {
    return config.find('_');
}

void speedThroughTheFinalState(State& currentState, int n, const std::vector<std::string>& importantHistory)
{
    std::vector<std::string> impHistory = importantHistory;
    std::string lastConfig = impHistory.back();
    impHistory.pop_back();
    std::string prevLastConfig = impHistory.back();
    std::string currentConfig = currentState.history.back();

    while (!impHistory.empty()) 
    {
        size_t indexOfLastEmptyLily = findIndexOfEmptyLily(lastConfig);
        size_t indexOfPrevLastEmptyLily = findIndexOfEmptyLily(prevLastConfig);

        std::swap(currentConfig[ n*2 - indexOfLastEmptyLily], currentConfig[n*2 - indexOfPrevLastEmptyLily]);
        currentState.history.push_back(currentConfig);
        impHistory.pop_back();
        lastConfig = prevLastConfig;
        if (!impHistory.empty()) {
            prevLastConfig = impHistory.back();
        }
    }
}

// Функция за решаване на задачата с BFS
std::vector<std::string> solveFrogLeapPuzzle(int n) {
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
    State initState(initialConfig, { initialConfig });
    q.push(initState);
    visited.insert(initialConfig);

    std::string finalConfiguration = generateFinalConfiguration(n);
    std::string midConfiguration = generateMidConfiguration(n);

    // DFS
    while (!q.empty()) {
        State currentState = q.top();
        q.pop();

        if (currentState.configuration == finalConfiguration) {
            result = currentState.history;
            result.push_back(currentState.configuration);
            break;
        }

        if (currentState.configuration == midConfiguration) {
            // Намерили сме средната конфигурация
            std::vector<std::string> importantHistory = currentState.history;
            // местим всички жаби, които гледат надясно с две позииции напред, като почваме от най-дясно стоящата
            makeMiddleMoves(currentState, n);
            // взимаме направени движения и ги обхождаме наобратно
            speedThroughTheFinalState(currentState, n, importantHistory);
            result = currentState.history;
            break;
        }

        std::vector<State> nextStates;
        generateNextStates(currentState, nextStates);

        for (const State& nextState : nextStates) {
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

void frogLeapDFS(State& currentState, const std::string& finalConfig )
{
  
    if (currentState.configuration == finalConfig)
    {
       // printResult(currentState.history);
        return;
    }

    std::string& currentConfig = currentState.configuration;
    int n = currentConfig.size();
    for (int i = 0; i < n; ++i)
    {
        if (currentConfig[i] == '<' && !isDeadEnd(currentConfig, n))
        {
            if (i > 0 && currentConfig[i - 1] == '_')
            {
                std::swap(currentConfig[i], currentConfig[i - 1]);

                State childState;
                std::vector<std::string> newHistory = currentState.history;
                newHistory.push_back(currentConfig);
                childState.history = newHistory;
                childState.configuration = currentConfig;

                frogLeapDFS(childState, finalConfig);

                std::swap(currentConfig[i], currentConfig[i - 1]);
            }

            if (i > 1 && currentConfig[i - 2] == '_') 
            {
                std::swap(currentConfig[i], currentConfig[i - 2]);
                State childState;
                std::vector<std::string> newHistory = currentState.history;
                newHistory.push_back(currentConfig);
                childState.history = newHistory;
                childState.configuration = currentConfig;

                frogLeapDFS(childState, finalConfig);

                std::swap(currentConfig[i], currentConfig[i - 2]);
            }
        }

        else if (currentConfig[i] == '>' && !isDeadEnd(currentConfig,n)) 
        {
            if (i < 2 * n - 1 && currentConfig[i + 1] == '_') 
            {
                std::swap(currentConfig[i], currentConfig[i + 1]);
                State childState;
                std::vector<std::string> newHistory = currentState.history;
                newHistory.push_back(currentConfig);
                childState.history = newHistory;
                childState.configuration = currentConfig;

                frogLeapDFS(childState, finalConfig);

                std::swap(currentConfig[i], currentConfig[i + 1]);
            }

            if (i < 2 * n - 2 && currentConfig[i + 2] == '_') 
            {
                std::swap(currentConfig[i], currentConfig[i + 2]);
                State childState;
                std::vector<std::string> newHistory = currentState.history;
                newHistory.push_back(currentConfig);
                childState.history = newHistory;
                childState.configuration = currentConfig;

                frogLeapDFS(childState, finalConfig);

                std::swap(currentConfig[i], currentConfig[i + 2]);
            }
        }
    }
}

std::string initialFrogs(int n)
{
    std::string initialConfig;
    
    for (int i = 0; i < n; ++i) {
        initialConfig += '>';
    }

    initialConfig += '_';

    for (int i = 0; i < n; ++i) {
        initialConfig += '<';
    }

    initialConfig += '\0';

    return initialConfig;
}




int main() {
    int n;
    std::cout << "Enter the number of frogs: ";
    std::cin >> n;

    State test;
    test.configuration = initialFrogs(n);
    std::string finalConfig = generateFinalConfiguration(n);
    //if (n >= 18)
    //{
    //    realNumOfFrogs = n - 5;
    //}
    //else
    //{
    //    realNumOfFrogs = n;
    //}
    // timer
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::seconds;
    
    auto t1 = high_resolution_clock::now();
    std::vector<std::string> result = solveFrogLeapPuzzle(n);
    auto t2 = high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << "s" << std::endl;


    t1 = high_resolution_clock::now();
    frogLeapDFS(test, finalConfig);
    t2 = high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << "s" << std::endl;



    //frogLeapDFS(test, finalConfig);
    //std::cout << "Solution steps:" << std::endl;
    //printResult(result);

   
    return 0;
}