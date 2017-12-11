//  The Magic Purse G-Research London
//  Andrzej Gruntowski
#include "LibsAndClassDeclarations.h"

#define RUN_UNIT_TESTS 0


namespace MagicPurse
{
    using CHILD_SUBSETS_TYPE = std::vector<std::pair<std::unordered_multiset<int>, std::unordered_multiset<int>>>;

    namespace Data
    {
        struct Denominations
        {
            int pounds   = 0;
            int shilling = 0;
            int pence    = 0;
        };
    }

    namespace Utils
    {
        bool isNumber(const std::string &input)
        {
            return !input.empty() && std::find_if(input.begin(), input.end(), [](char c) { return !std::isdigit(c); }) == input.end();
        }

        void printDenominationsData(const Data::Denominations &denominationsData)
        {
            std::cout << denominationsData.pounds << std::endl;
            std::cout << denominationsData.shilling << std::endl;
            std::cout << denominationsData.pence << std::endl;
        }

        bool isEvenNumber(size_t number)
        {
            return !(number & 1);
        }

        size_t getNumberOfDelimiters(const std::string &input, const char delimiter)
        {
            return std::count(input.begin(), input.end(), delimiter);
        }
    }

    //Perform input argument
    void setNumberOfParticularDenominationFromInputContainsTwoDelimiters(Data::Denominations &denominationsData, const std::string &inputData, const char delimiter)
    {
        size_t firstDelimiterOccuring = inputData.find(delimiter);
        size_t secondDelimiterOccuring = inputData.find_last_of(delimiter);

        std::string inputDataSubstr(inputData.substr(0, firstDelimiterOccuring));
        if(Utils::isNumber(inputDataSubstr)) denominationsData.pounds = stoi(inputDataSubstr);

        inputDataSubstr = inputData.substr(firstDelimiterOccuring + 1, secondDelimiterOccuring - (firstDelimiterOccuring + 1));
        if(Utils::isNumber(inputDataSubstr)) denominationsData.shilling = stoi(inputDataSubstr);

        inputDataSubstr = inputData.substr(secondDelimiterOccuring + 1, inputData.length() - secondDelimiterOccuring);
        if(Utils::isNumber(inputDataSubstr)) denominationsData.pence = stoi(inputDataSubstr);
    }

    void setNumberOfParticularDenominationFromInputContainsOneDelimiter(Data::Denominations &denominationsData, const std::string &inputData, const char delimiter)
    {
        size_t delimiterPosition = inputData.find(delimiter);

        std::string inputDataSubstr(inputData.substr(0, delimiterPosition));
        if(Utils::isNumber(inputDataSubstr)) denominationsData.shilling = stoi(inputDataSubstr);

        inputDataSubstr = inputData.substr(delimiterPosition + 1, inputData.length() - delimiterPosition);
        if(Utils::isNumber(inputDataSubstr)) denominationsData.pence = stoi(inputDataSubstr);
    }

    void handleInputData(Data::Denominations &denominationsData, const std::string &inputData)
    {
        const char delimiter = '/';
        size_t numberOfSlashes = Utils::getNumberOfDelimiters(inputData, delimiter);
        if(numberOfSlashes == 2) setNumberOfParticularDenominationFromInputContainsTwoDelimiters(denominationsData, inputData, delimiter);
        else if(numberOfSlashes == 1) setNumberOfParticularDenominationFromInputContainsOneDelimiter(denominationsData, inputData, delimiter);
    }

    int getNumberOfSplitDenominations(const Data::Denominations &denominationsData)
    {
        int numberOfCoinsToPerform = 0;
        numberOfCoinsToPerform += denominationsData.pounds * 20 * 12;
        numberOfCoinsToPerform += denominationsData.shilling * 12;
        numberOfCoinsToPerform += denominationsData.pence;
        numberOfCoinsToPerform *= 2;        //for that solution!
        return numberOfCoinsToPerform;
    }

    //Solution
    int performCombinations(int amountOfCoins, std::vector<int> &currentCombination, std::vector<std::vector<int>> &allCombinations)
    {
        if (amountOfCoins < 0) return 0;
        if (amountOfCoins == 0)
        {
            allCombinations.push_back(currentCombination);
            return 1;
        }

        std::vector<int> denomination_1(currentCombination);
        denomination_1.push_back(1);
        std::vector<int> denomination_2(currentCombination);
        denomination_2.push_back(2);
        std::vector<int> denomination_6(currentCombination);
        denomination_6.push_back(6);
        std::vector<int> denomination_12(currentCombination);
        denomination_12.push_back(12);

        return performCombinations(amountOfCoins - 1, denomination_1, allCombinations) + performCombinations(amountOfCoins - 2, denomination_2, allCombinations) + performCombinations(amountOfCoins - 6, denomination_6, allCombinations) + performCombinations(amountOfCoins - 12, denomination_12, allCombinations);
    }

    int getNumberOfFilteredCombinations(std::vector<std::vector<int>> &allCombinations)
    {
        CHILD_SUBSETS_TYPE childSubsets;

        for(const auto &currentPermutation : allCombinations)
        {
            if(Utils::isEvenNumber(currentPermutation.size()))
            {
                size_t currentPermutationSize = currentPermutation.size();
                size_t currentPermutationHalfSize = currentPermutationSize / 2;

                //Left
                std::unordered_multiset<int> leftSet;

                for(int i=0;i<currentPermutationHalfSize;++i)
                    leftSet.insert(currentPermutation[i]);

                bool leftSubsetExist = false;
                for(const auto &subset : childSubsets)
                {
                    if(subset.first == leftSet)
                    {
                        leftSubsetExist = true;
                        break;
                    }
                }

                //Right
                std::unordered_multiset<int> rightSet;

                for(int i = currentPermutationHalfSize; i < currentPermutationSize; ++i)
                    rightSet.insert(currentPermutation[i]);

                bool rightSubsetExist = false;
                for(const auto &subset : childSubsets)
                {
                    if(subset.second == rightSet)
                    {
                        rightSubsetExist = true;
                        break;
                    }
                }

                //Summarize
                if(!leftSubsetExist || !rightSubsetExist) childSubsets.push_back({leftSet, rightSet});
            }
        }
        return childSubsets.size();
    }

    void magicPurseSolution(int amountOfCoins)
    {
        std::vector<int> currentCombination;
        std::vector<std::vector<int>> allCombinations;
        //std::cout << "Number_of_all_combinations=" << performCombinations(amountOfCoins, currentCombination, allCombinations) << std::endl;
        //std::cout << "Number_of_filtered_combinations=" << getNumberOfFilteredCombinations(allCombinations) << std::endl;
        performCombinations(amountOfCoins, currentCombination, allCombinations);
        std::cout << getNumberOfFilteredCombinations(allCombinations) << std::endl;
    }

    namespace Tests
    {
        //Perform input argument
        bool checkExpectedDenominationsData(const Data::Denominations &currentDenominationsData, const Data::Denominations &expectedDenominationsData)
        {
            return (currentDenominationsData.pounds == expectedDenominationsData.pounds) &&
                   (currentDenominationsData.shilling == expectedDenominationsData.shilling) &&
                   (currentDenominationsData.pence == expectedDenominationsData.pence);
        }

        void isNumberTest()
        {
            std::cout << std::boolalpha << Utils::isNumber("0") << std::endl;  //true
            std::cout << Utils::isNumber("2048") << std::endl;                 //true
            std::cout << Utils::isNumber("G-Research London") << std::endl;    //false
        }

        void setNumberOfParticularDenominationFromInputContainsTwoDelimitersTest()
        {
            const char delimiter = '/';

            //1
            Data::Denominations denominationsData;
            setNumberOfParticularDenominationFromInputContainsTwoDelimiters(denominationsData, "256/32/64", delimiter);

            std::cout << std::boolalpha << denominationsData.pounds << std::endl;   //256
            std::cout << denominationsData.shilling << std::endl;                   //32
            std::cout << denominationsData.pence << std::endl;                      //64

            //2
            denominationsData = {};
            setNumberOfParticularDenominationFromInputContainsTwoDelimiters(denominationsData, "0/0/0", delimiter);

            std::cout << denominationsData.pounds << std::endl;                     //0
            std::cout << denominationsData.shilling << std::endl;                   //0
            std::cout << denominationsData.pence << std::endl;                      //0

            //3
            denominationsData = {};
            setNumberOfParticularDenominationFromInputContainsTwoDelimiters(denominationsData, "G-Research London", delimiter);

            std::cout << denominationsData.pounds << std::endl;                     //0
            std::cout << denominationsData.shilling << std::endl;                   //0
            std::cout << denominationsData.pence << std::endl;                      //0
        }

        void setNumberOfParticularDenominationFromInputContainsOneDelimiterTest()
        {
            const char delimiter = '/';

            //1
            Data::Denominations denominationsData;
            setNumberOfParticularDenominationFromInputContainsOneDelimiter(denominationsData, "32/64", delimiter);

            std::cout << std::boolalpha << denominationsData.pounds << std::endl;   //0
            std::cout << denominationsData.shilling << std::endl;                   //32
            std::cout << denominationsData.pence << std::endl;                      //64

            //2
            denominationsData = {};
            setNumberOfParticularDenominationFromInputContainsOneDelimiter(denominationsData, "0/0", delimiter);

            std::cout << denominationsData.pounds << std::endl;                     //0
            std::cout << denominationsData.shilling << std::endl;                   //0
            std::cout << denominationsData.pence << std::endl;                      //0

            //3
            denominationsData = {};
            setNumberOfParticularDenominationFromInputContainsOneDelimiter(denominationsData, "G-Research London", delimiter);

            std::cout << denominationsData.pounds << std::endl;                     //0
            std::cout << denominationsData.shilling << std::endl;                   //0
            std::cout << denominationsData.pence << std::endl;                      //0
        }

        void getNumberOfDelimitersTest()
        {
            std::cout << Utils::getNumberOfDelimiters("32/256/2048", '/') << std::endl;    //2
            std::cout << Utils::getNumberOfDelimiters("32/-/2048", '/') << std::endl;      //2
            std::cout << Utils::getNumberOfDelimiters("-/-/-", '/') << std::endl;          //2
            std::cout << Utils::getNumberOfDelimiters("-/-", '/') << std::endl;            //1
            std::cout << Utils::getNumberOfDelimiters("", '/') << std::endl;               //0
        }

        void handleInputDataTest()
        {
            //1
            std::string inputData("32/256/2048");
            Data::Denominations denominationsData;
            Data::Denominations expectedDenominationsData = {32, 256, 2048};
            handleInputData(denominationsData, inputData);
            std::cout << std::boolalpha << checkExpectedDenominationsData(denominationsData, expectedDenominationsData) << std::endl;   //true

            //2
            inputData = "256/32";
            denominationsData = {};
            handleInputData(denominationsData, inputData);
            expectedDenominationsData = {0, 256, 32};
            std::cout << checkExpectedDenominationsData(denominationsData, expectedDenominationsData) << std::endl;                     //true

            //3
            inputData = "-/-";
            denominationsData = {};
            handleInputData(denominationsData, inputData);
            expectedDenominationsData = {0, 0, 0};
            std::cout << checkExpectedDenominationsData(denominationsData, expectedDenominationsData) << std::endl;                     //true

            //4
            inputData = "-/-/-";
            denominationsData = {};
            handleInputData(denominationsData, inputData);
            expectedDenominationsData = {0, 0, 0};
            std::cout << checkExpectedDenominationsData(denominationsData, expectedDenominationsData) << std::endl;                     //true
        }

        void getNumberOfSplitDenominationsTest()
        {
            //1
            Data::Denominations denominationsData = {0, 0, 0};
            std::cout << getNumberOfSplitDenominations(denominationsData) << std::endl; //0

            //2
            denominationsData = {2, 4, 8};
            std::cout << getNumberOfSplitDenominations(denominationsData) << std::endl; //1072

            //3
            denominationsData = {2, 0, 0};
            std::cout << getNumberOfSplitDenominations(denominationsData) << std::endl; //960

            //4
            denominationsData = {0, 4, 0};
            std::cout << getNumberOfSplitDenominations(denominationsData) << std::endl; //96
        }

        //Solution
        void isEvenNumberTest()
        {
            std::cout << std::boolalpha << Utils::isEvenNumber(0) << std::endl;    //true
            std::cout << Utils::isEvenNumber(1024) << std::endl;                   //true
            std::cout << Utils::isEvenNumber(2047) << std::endl;                   //false
        }

        void performCombinationsTest()
        {
            //1
            int amountOfCoins = 0;
            std::vector<int> currentCombination;
            std::vector<std::vector<int>> allCombinations;
            std::cout << performCombinations(amountOfCoins, currentCombination, allCombinations) << std::endl;    //1

            //2
            amountOfCoins = 6;
            currentCombination.clear();
            allCombinations.clear();
            std::cout << performCombinations(amountOfCoins, currentCombination, allCombinations) << std::endl;    //14
        }

        void getNumberOfFilteredCombinationsTest()
        {
            //1
            int amountOfCoins = 0;
            std::vector<int> currentCombination;
            std::vector<std::vector<int>> allCombinations;
            performCombinations(amountOfCoins, currentCombination, allCombinations);
            std::cout << getNumberOfFilteredCombinations(allCombinations) << std::endl;   //1

            //2
            amountOfCoins = 6;
            currentCombination.clear();
            allCombinations.clear();
            performCombinations(amountOfCoins, currentCombination, allCombinations);
            std::cout << getNumberOfFilteredCombinations(allCombinations) << std::endl;   //4
        }

        void runTests()
        {
            //Perform input argument
            isNumberTest();
            setNumberOfParticularDenominationFromInputContainsTwoDelimitersTest();
            setNumberOfParticularDenominationFromInputContainsOneDelimiterTest();
            getNumberOfDelimitersTest();
            handleInputDataTest();

            //Solution
            isEvenNumberTest();
            performCombinationsTest();
            getNumberOfFilteredCombinationsTest();
        }
    }
}


int main(int argc, char **argv)
{
    if(argc == 2)
    {
        std::string inputData(argv[1]);
        MagicPurse::Data::Denominations denominationsData;

        MagicPurse::handleInputData(denominationsData, inputData);
        int amountOfCoins = MagicPurse::getNumberOfSplitDenominations(denominationsData);
        MagicPurse::magicPurseSolution(amountOfCoins);
    }

    #if RUN_UNIT_TESTS
        MagicPurse::Tests::runTests();
    #endif

    return 0;
}

//clear && mkdir build && cmake .. && make && g++ Main.cpp -o Main && ./Main -/-/3     //4
