#pragma once

#include <string>
#include <ModelInput.h>

class PromptGenerator {
public:

    PromptGenerator(){}
    PromptGenerator(const PromptGenerator& other){}
    PromptGenerator(const PromptGenerator&& other){}

    static std::string shorthandInstruction() {
        return "Use bullet points and write in shorthand. For example, \"O(n log n) due to sorting\" is preferred to \"The time complexity of the implementation is O(n log n) due to the sorting.\"";
    }

    static std::string domain(){
        return "software engineering";
    }
    
    static std::string systemMessage() {
        return "You are a " + domain() + " expert.";
    }
    
    virtual ModelInput extractQuestion(std::string transcript) {
        auto prompt =
"Extract the last problem or question posed by the interviewer during a " + domain() + " interview. State it as an instruction. If the question is about something the candidate did, restate it in a general way.\n\
\n\
[transcript begins]\n\
If you want to improve the query performance of multiple columns or a group of columns in a given table. Cool. And is it considered a cluster index or no cluster index? definitely be a non-clustered index. For sure. All right, great. So next question. What's the difference between \"where\" and \"having\"? Oh, that's an interesting one.\n\
[transcript ends]\n\
Is context needed here: Yes\n\
Context: queries, databases, performance\n\
Extracted question: Describe the difference between \"where\" and \"having\" clauses in SQL, focusing on performance.\n\
Answer in code: No\n\
\n\
[transcript begins]\n\
Are you familiar with the traceroute command? Yes I am. Okay, so how does that work behind the scenes?\n\
[transcript ends]\n\
Is context needed here: No\n\
Extracted question: How does the traceroute command work?\n\
Answer in code: No\n\
\n\
[transcript begins]\n\
Write a function that takes 3 arguments. The first argument is a list of numbers that is guaranteed to be sorted. The remaining two arguments, a and b, are the coefficients of the function f(x) = a*x + b. Your function should compute f(x) for every number in the first argument, and return a list of those values, also sorted.\n\
[transcript ends]\n\
Is context needed here: Yes\n\
Context: C++\n\
Extracted question: C++ function that takes a vector of sorted numbers; and coefficients (a, b) of the function f(x) = a*x + b. It should compute f(x) for each input number, and return a sorted vector.\n\
Answer in code: Yes\n\
\n\
[transcript begins]\n\
" + transcript + " \n\
[transcript ends]\n\
Is context needed here:\n\
";
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
    
    ModelInput answerPromptQuestion(std::string question) {
        auto prompt =
"You are a " + domain() + " expert. " + shorthandInstruction() +
"\n\
Example 1:\n\
Question: Should I use \"where\" or \"having\" to find employee first names that appear more than 250 times?\n\
Are follow up questions needed here: Yes\n\
Follow up: Will this query use aggregation?\n\
Intermediate answer: Yes, count(first_name)\n\
Follow up: Does \"where\" or \"having\" filter rows after aggregation?\n\
Intermediate answer: having\n\
Final answer:\n\
• Where: filters rows before aggregation\n\
• Having: filters rows after aggregation\n\
• Example SQL: having count(first_name) > 250\n\
\n\
Example 2:\n\
Question: How does the traceroute command work?\n\
Are follow up questions needed here: No\n\
Final answer:\n\
• Traces the path an IP packet takes across networks\n\
• Starting from 1, increments the TTL field in the IP header\n\
• The returned ICMP Time Exceeded packets are used to build a list of routers\n\
\n\
Question: " + question;
        
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
    
    ModelInput answerPreviousQuestion(std::string question, std::string previousAnswer ) {
        auto prompt =
"You are a " + domain() + " expert. Refine the partial answer. " + shorthandInstruction() +
"\n\
Example 1:\n\
Question: Should I use \"where\" or \"having\" to find employee first names that appear more than 250 times?\n\
Partial answer:\n\
• Having: filters rows after aggregation\n\
Are follow up questions needed here: Yes\n\
Follow up: Will this query use aggregation?\n\
Intermediate answer: Yes, count(first_name)\n\
Follow up: Does \"where\" or \"having\" filter rows after aggregation?\n\
Intermediate answer: having\n\
Final answer:\n\
• Where: filters rows before aggregation\n\
• Having: filters rows after aggregation\n\
• Example SQL: having count(first_name) > 250\n\
\n\
Example 2:\n\
Question: How does the traceroute command work?\n\
Partial answer:\n\
• Traces the path an IP packet takes across networks\n\
• Starting from 1, increments the TTL field in the IP header\n\
Are follow up questions needed here: No\n\
Final answer:\n\
• Traces the path an IP packet takes across networks\n\
• Starting from 1, increments the TTL field in the IP header\n\
• The returned ICMP Time Exceeded packets are used to build a list of routers\n\
\n\
Question: " + question +
"\n\
Partial answer:\n" + previousAnswer;
        
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
    
    ModelInput answerHighlightedQuestion(std::string question, std::string highlightedAnswer) {
        auto prompt = 
"Question: " + question + "\n\
\n\
You previously provided this answer, and I have highlighted part of it:\n\
" + highlightedAnswer + "\n\
\n\
Explain the highlighted part of your previous answer in much greater depth. " + shorthandInstruction();
        
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
    
    ModelInput writeCode(std::string task) {
        auto prompt = 
"Write pseudocode to accomplish this task: " + task + "\n\
\n\
Start with a comment outlining opportunities for optimization and potential pitfalls. Assume only standard libraries are available, unless specified. Don't explain, just give me the code.\n\
";        
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
     
    ModelInput analyzeBrowserCode(std::string code, std::string logs, std::string task = "") {
        std::string prefix;
        if (!task.empty()) {
            prefix = "Prompt: " + task;
        } else {
            prefix = "Briefly describe how an efficient solution can be achieved.";
        }
        
        auto prompt = 
prefix + "\n\
\n\
Code:\n\
" + code + "\n\
\n\
Output:\n\
" + logs + "\n\
\n\
                      If the prompt is irrelevant, you may disregard it. You may suggest edits to the existing code. If appropriate, include a brief discussion of complexity." + shorthandInstruction() + "\n\
";      
        return ModelInput(systemMessage(), prompt, OpenAIModelType::chat_chatgpt, "");
    }
};
