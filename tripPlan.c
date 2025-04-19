/*
 * tripPlan.c - 旅行规划程序
 * 
 * 时间复杂度分析:
 * 设n为地标数量，m为步行连接数量，f为渡轮时刻表数量，q为查询数量
 * 
 * 预处理阶段:
 * - 读取所有地标 O(n)
 * - 构建所有步行连接 O(m)
 * - 构建所有渡轮连接 O(f)
 * 
 * 路径查找阶段:
 * - 使用Dijkstra算法找最短路径 O(n²)，如果使用邻接矩阵
 * - 每次查询时间复杂度为O(n² + m + f)
 * 
 * 总体时间复杂度: O(n + m + f + q*(n² + m + f))
 * 但由于查询处理过程中不需要重复处理所有步行连接和渡轮连接，
 * 可以简化为: O(n + m + f + q*n²)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_LANDMARKS 100
#define MAX_NAME_LEN 32
#define MINUTES_PER_DAY 1440
#define NO_ROUTE "No route.\n"

// 表示四位数时间 (hhmm)
typedef int Time;

// 将hhmm格式的时间转换为分钟数 (从00:00开始)
int timeToMinutes(Time time) {
    int hours = time / 100;
    int minutes = time % 100;
    return hours * 60 + minutes;
}

// 将分钟数转换为hhmm格式的时间
Time minutesToTime(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    return hours * 100 + mins;
}

// 定义地标
typedef struct {
    char name[MAX_NAME_LEN];
} Landmark;

// 步行连接
typedef struct {
    int from;               // 起始地标索引
    int to;                 // 目标地标索引
    int walkingTime;        // 步行时间（分钟）
} WalkingLink;

// 渡轮时刻
typedef struct {
    int from;               // 出发地标索引
    int to;                 // 到达地标索引
    Time departureTime;     // 出发时间 (hhmm)
    Time arrivalTime;       // 到达时间 (hhmm)
    int departureMinutes;   // 出发时间（分钟）
    int arrivalMinutes;     // 到达时间（分钟）
    int travelTime;         // 旅行时间（分钟）
} FerrySchedule;

// 路径节点类型
enum RouteType {
    WALK,
    FERRY
};

// 路径节点
typedef struct RouteNode {
    enum RouteType type;
    int fromLandmark;
    int toLandmark;
    int departureMinutes;
    int arrivalMinutes;
    int duration;
    struct RouteNode *next;
} RouteNode;

// 全局变量
Landmark landmarks[MAX_LANDMARKS];        // 地标数组
int numLandmarks = 0;                     // 地标数量
WalkingLink *walkingLinks = NULL;         // 步行连接数组
int numWalkingLinks = 0;                  // 步行连接数量
FerrySchedule *ferrySchedules = NULL;     // 渡轮时刻表数组
int numFerrySchedules = 0;                // 渡轮时刻表数量

// 函数声明
int findLandmarkIndex(const char *name);
void readLandmarks();
void readWalkingLinks();
void readFerrySchedules();
RouteNode* findRoute(int fromLandmark, int toLandmark, int departureMinutes);
void printRoute(RouteNode *route);
void freeRoute(RouteNode *route);

// 主函数
int main() {
    // 读取地标
    printf("Number of landmarks: ");
    scanf("%d", &numLandmarks);
    
    for (int i = 0; i < numLandmarks; i++) {
        scanf("%s", landmarks[i].name);
    }
    
    // 读取步行连接
    printf("Number of walking links: ");
    scanf("%d", &numWalkingLinks);
    
    walkingLinks = malloc(numWalkingLinks * sizeof(WalkingLink));
    
    for (int i = 0; i < numWalkingLinks; i++) {
        char fromName[MAX_NAME_LEN];
        char toName[MAX_NAME_LEN];
        int walkingTime;
        
        scanf("%s", fromName);
        scanf("%s", toName);
        scanf("%d", &walkingTime);
        
        walkingLinks[i].from = findLandmarkIndex(fromName);
        walkingLinks[i].to = findLandmarkIndex(toName);
        walkingLinks[i].walkingTime = walkingTime;
    }
    
    // 读取渡轮时刻表
    printf("Number of ferry schedules: ");
    scanf("%d", &numFerrySchedules);
    
    ferrySchedules = malloc(numFerrySchedules * sizeof(FerrySchedule));
    
    for (int i = 0; i < numFerrySchedules; i++) {
        char fromName[MAX_NAME_LEN];
        char toName[MAX_NAME_LEN];
        Time departureTime, arrivalTime;
        
        scanf("%s", fromName);
        scanf("%d", &departureTime);
        scanf("%s", toName);
        scanf("%d", &arrivalTime);
        
        ferrySchedules[i].from = findLandmarkIndex(fromName);
        ferrySchedules[i].to = findLandmarkIndex(toName);
        ferrySchedules[i].departureTime = departureTime;
        ferrySchedules[i].arrivalTime = arrivalTime;
        ferrySchedules[i].departureMinutes = timeToMinutes(departureTime);
        ferrySchedules[i].arrivalMinutes = timeToMinutes(arrivalTime);
        ferrySchedules[i].travelTime = ferrySchedules[i].arrivalMinutes - ferrySchedules[i].departureMinutes;
    }
    
    // 处理用户查询
    while (1) {
        char fromName[MAX_NAME_LEN];
        
        printf("\nFrom: ");
        scanf("%s", fromName);
        
        // 检查是否结束
        if (strcmp(fromName, "done") == 0) {
            printf("Happy travels!\n");
            break;
        }
        
        char toName[MAX_NAME_LEN];
        int departureTime;
        
        printf("To: ");
        scanf("%s", toName);
        
        printf("Departure time: ");
        scanf("%d", &departureTime);
        
        int fromIndex = findLandmarkIndex(fromName);
        int toIndex = findLandmarkIndex(toName);
        int departureMinutes = timeToMinutes(departureTime);
        
        // 寻找路线
        RouteNode *route = findRoute(fromIndex, toIndex, departureMinutes);
        
        // 打印路线
        printf("\n");
        if (route) {
            printRoute(route);
            freeRoute(route);
        } else {
            printf(NO_ROUTE);
        }
    }
    
    // 释放内存
    free(walkingLinks);
    free(ferrySchedules);
    
    return 0;
}

// 根据地标名称查找索引
int findLandmarkIndex(const char *name) {
    for (int i = 0; i < numLandmarks; i++) {
        if (strcmp(landmarks[i].name, name) == 0) {
            return i;
        }
    }
    return -1;  // 未找到
}

// 添加路径节点
RouteNode* addRouteNode(RouteNode *head, enum RouteType type, int from, int to, 
                        int departureMinutes, int arrivalMinutes, int duration) {
    RouteNode *newNode = malloc(sizeof(RouteNode));
    newNode->type = type;
    newNode->fromLandmark = from;
    newNode->toLandmark = to;
    newNode->departureMinutes = departureMinutes;
    newNode->arrivalMinutes = arrivalMinutes;
    newNode->duration = duration;
    newNode->next = NULL;
    
    if (head == NULL) {
        return newNode;
    }
    
    RouteNode *curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = newNode;
    
    return head;
}

// 寻找路线（使用Dijkstra算法）
RouteNode* findRoute(int fromLandmark, int toLandmark, int departureMinutes) {
    // 初始化距离数组和前驱节点数组
    int dist[MAX_LANDMARKS];
    int prev[MAX_LANDMARKS];
    enum RouteType prevType[MAX_LANDMARKS];  // 记录前一步是步行还是渡轮
    int prevDepartureTime[MAX_LANDMARKS];    // 记录前一步的出发时间
    int ferry[MAX_LANDMARKS];                // 记录使用的渡轮索引
    bool visited[MAX_LANDMARKS];
    
    // 初始化
    for (int i = 0; i < numLandmarks; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        prevType[i] = WALK;
        prevDepartureTime[i] = -1;
        ferry[i] = -1;
        visited[i] = false;
    }
    
    // 设置起点
    dist[fromLandmark] = departureMinutes;
    
    // Dijkstra算法
    for (int count = 0; count < numLandmarks; count++) {
        // 找到距离最小的未访问节点
        int minDist = INT_MAX;
        int u = -1;
        
        for (int i = 0; i < numLandmarks; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        // 如果没有可访问的节点，或者已经到达目标地标，则退出
        if (u == -1 || u == toLandmark) break;
        
        // 标记为已访问
        visited[u] = true;
        
        // 更新邻居节点的距离
        // 1. 通过步行
        for (int i = 0; i < numWalkingLinks; i++) {
            if (walkingLinks[i].from == u) {
                int v = walkingLinks[i].to;
                int newDist = dist[u] + walkingLinks[i].walkingTime;
                
                if (!visited[v] && newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    prevType[v] = WALK;
                    prevDepartureTime[v] = dist[u];
                }
            } else if (walkingLinks[i].to == u) {
                // 步行连接是双向的
                int v = walkingLinks[i].from;
                int newDist = dist[u] + walkingLinks[i].walkingTime;
                
                if (!visited[v] && newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    prevType[v] = WALK;
                    prevDepartureTime[v] = dist[u];
                }
            }
        }
        
        // 2. 通过渡轮
        for (int i = 0; i < numFerrySchedules; i++) {
            if (ferrySchedules[i].from == u && 
                ferrySchedules[i].departureMinutes >= dist[u]) { // 确保渡轮出发时间晚于到达时间
                
                int v = ferrySchedules[i].to;
                int newDist = ferrySchedules[i].arrivalMinutes;
                
                if (!visited[v] && newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    prevType[v] = FERRY;
                    prevDepartureTime[v] = ferrySchedules[i].departureMinutes;
                    ferry[v] = i;  // 记录使用的渡轮
                }
            }
        }
    }
    
    // 如果没有路径到达目标地标
    if (dist[toLandmark] == INT_MAX) {
        return NULL;
    }
    
    // 构建路径（从终点回溯到起点）
    RouteNode *route = NULL;
    int current = toLandmark;
    
    while (current != fromLandmark) {
        int previous = prev[current];
        
        if (prevType[current] == WALK) {
            // 添加步行路径节点
            int walkTime = dist[current] - prevDepartureTime[current];
            route = addRouteNode(route, WALK, previous, current, 
                                prevDepartureTime[current], dist[current], walkTime);
        } else {
            // 添加渡轮路径节点
            int ferryIndex = ferry[current];
            route = addRouteNode(route, FERRY, previous, current, 
                                ferrySchedules[ferryIndex].departureMinutes,
                                ferrySchedules[ferryIndex].arrivalMinutes,
                                ferrySchedules[ferryIndex].travelTime);
        }
        
        current = previous;
    }
    
    // 反转路径（从起点到终点）
    RouteNode *reversedRoute = NULL;
    RouteNode *node = route;
    
    while (node != NULL) {
        RouteNode *next = node->next;
        node->next = reversedRoute;
        reversedRoute = node;
        node = next;
    }
    
    return reversedRoute;
}

// 打印路径
void printRoute(RouteNode *route) {
    RouteNode *current = route;
    
    while (current != NULL) {
        if (current->type == WALK) {
            printf("Walk %d minute(s):\n", current->duration);
            printf("  %04d %s\n", minutesToTime(current->departureMinutes), landmarks[current->fromLandmark].name);
            printf("  %04d %s\n", minutesToTime(current->arrivalMinutes), landmarks[current->toLandmark].name);
        } else {
            printf("Ferry %d minute(s):\n", current->duration);
            printf("  %04d %s\n", minutesToTime(current->departureMinutes), landmarks[current->fromLandmark].name);
            printf("  %04d %s\n", minutesToTime(current->arrivalMinutes), landmarks[current->toLandmark].name);
        }
        
        current = current->next;
    }
}

// 释放路径内存
void freeRoute(RouteNode *route) {
    RouteNode *current = route;
    while (current != NULL) {
        RouteNode *next = current->next;
        free(current);
        current = next;
    }
} 