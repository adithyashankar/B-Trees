/*
���ؼ��ֵ�˳�����B-��:
(3, 11)
(7, 16)
(12, 4)
(24, 1)
(26, 13)
(30, 12)
(37, 5)
(45, 2)
(50, 6)
(53, 3)
(61, 7)
(70, 10)
(85, 14)
(90, 8)
(100, 9)
 
����������Ҽ�¼�Ĺؼ���: 26
(26, 13)
 
5
û�ҵ�
 
37
(37, 5)
 
*/
 
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
using namespace std;
 
#define m 3  // B���Ľף�����Ϊ3
//3�׵�B-�������з��յ�����������������ؼ���
#define N 16 // ����Ԫ�ظ���
#define MAX 5 // �ַ�����󳤶� + 1  
 
//��¼����
struct Record{
    int key; // �ؼ���
    char info[MAX];
};  
 
//B-��ADT
struct BTreeNode {
    int keynum; // ����йؼ��ָ���
    struct BTreeNode * parent; // ָ��˫�׽��
    struct Node { // �������
        int key; // �ؼ���
        Record * recptr; // ��¼ָ��
        struct BTreeNode * ptr; // ����ָ��
    }node[m + 1]; // key, recptr��0�ŵ�Ԫδ��
}; 
 
typedef BTreeNode BT;
typedef BTreeNode * Position;
typedef BTreeNode * SearchTree;
 
//B-�����ҽ��������
typedef struct {
    Position pt; // ָ���ҵ��Ľ��
    int i; // 1..m���ڽ���еĹؼ������
    int tag; // 1:���ҳɹ���O:����ʧ��
}Result; 
 
inline void print(BT c, int i) {// TraverseSearchTree()���õĺ���
    printf("(%d, %s)\n", c.node[i].key, c.node[i].recptr->info);
}
 
//���ٲ�����
void DestroySearchTree(SearchTree tree) {
    if(tree) {// �ǿ���
        for(int i = 0; i <= (tree)->keynum; i++ ) {
            DestroySearchTree(tree->node[i].ptr); // �������ٵ�i������
        }
        free(tree); // �ͷŸ����
        tree = NULL; // ��ָ�븳0
    }
}
 
//��p->node[1..keynum].key�в���i, ʹ��p->node[i].key��K��p->node[i + 1].key
//���ظպ�С�ڵ���K��λ��
int Search(Position p, int K) {
    int location = 0;
    for(int i = 1; i <= p->keynum; i++ ) {
        if(p->node[i].key <= K) {
            location = i;
        }
    }
    return location;
}
 
/*
��m��B��tree�ϲ��ҹؼ���K�����ؽ��(pt, i, tag)��
�����ҳɹ���tag = 1��ָ��pt��ָ����е�i���ؼ��ֵ���K��
������ʧ�ܣ�tag = 0������K�Ĺؼ���Ӧ������ָ��Pt��ָ����е�i�͵�i + 1���ؼ���֮�䡣
*/
Result SearchPosition(SearchTree tree, int K) {
    Position p = tree, q = NULL; // ��ʼ����pָ������㣬qָ��p��˫��
    bool found = false;
    int i = 0;
    Result r;
    while(p && !found) {
        i = Search(p, K); // p->node[i].key��K<p->node[i + 1].key
        if(i > 0 && p->node[i].key == K) {// �ҵ�����ؼ���
            found = true;
        } else {
            q = p;
            p = p->node[i].ptr;
        }
    }
    r.i = i;
    if(found) {// ���ҳɹ�
        r.pt = p;
        r.tag = 1;
    } else {//  ���Ҳ��ɹ�������K�Ĳ���λ����Ϣ
        r.pt = q;
        r.tag = 0;
    }
    return r;
}
 
//��r->key��r��ap�ֱ���뵽q->key[i + 1]��q->recptr[i + 1]��q->ptr[i + 1]��
void Insert(Position q, int i, Record * r, Position ap) {
    for(int j = q->keynum; j > i; j--) {// �ճ�q->node[i + 1]
        q->node[j + 1] = q->node[j];
    }
    q->node[i + 1].key = r->key;
    q->node[i + 1].ptr = ap;
    q->node[i + 1].recptr = r;
    q->keynum++;
}
 
// �����q���ѳ�������㣬ǰһ�뱣������һ�������������ap
void split(Position &q, Position &ap) {
    int s = (m + 1) / 2;
    ap = (Position)malloc(sizeof(BT)); // �����½��ap
    ap->node[0].ptr = q->node[s].ptr; // ��һ������ap
    for(int i = s + 1; i <= m; i++ ) {
        ap->node[i-s] = q->node[i];
        if(ap->node[i - s].ptr) {
            ap->node[i - s].ptr->parent = ap;
        }
    }
    ap->keynum = m - s;
    ap->parent = q->parent;
    q->keynum = s - 1; // q��ǰһ�뱣�����޸�keynum
}
 
// ���ɺ���Ϣ(T, r, ap)���µĸ����*T��ԭT��apΪ����ָ��
void NewRoot(Position &tree, Record *r, Position ap) {
    Position p;
    p = (Position)malloc(sizeof(BT));
    p->node[0].ptr = tree;
    tree = p;
    if(tree->node[0].ptr) {
        tree->node[0].ptr->parent = tree;
    }
    tree->parent = NULL;
    tree->keynum = 1;
    tree->node[1].key = r->key;
    tree->node[1].recptr = r;
    tree->node[1].ptr = ap;
    if(tree->node[1].ptr) {
        tree->node[1].ptr->parent = tree;
    }
}
 
/*
��m��B-��tree�Ͻ��*q��key[i]��key[i + 1]֮�����ؼ���K��ָ��r��������
������, ����˫�������б�Ҫ�Ľ����ѵ���, ʹtree����m��B����
*/
void InsertPosition(SearchTree &tree, Record &r, Position q, int i) {
    Position ap = NULL;
    bool finished = false;
    Record *rx = &r;
 
    while(q && !finished) {
        // ��r->key��r��ap�ֱ���뵽q->key[i + 1]��q->recptr[i + 1]��q->ptr[i + 1]��
        Insert(q, i, rx, ap);
        if(q->keynum < m) {
            finished = true; // �������
        } else { // ���ѽ��*q
            int s = (m + 1) >> 1;
            rx = q->node[s].recptr;
            // ��q->key[s + 1..m], q->ptr[s..m]��q->recptr[s + 1..m]�����½��*ap
            split(q, ap);
            q = q->parent;
            if(q) {
                i = Search(q, rx->key); // ��˫�׽��*q�в���rx->key�Ĳ���λ��
            }
        }
    }
    if(!finished) {// T�ǿ���(����q��ֵΪNULL)�������ѷ���Ϊ���*q��*ap
        NewRoot(tree, rx, ap); // ���ɺ���Ϣ(T, rx, ap)���µĸ����*T��ԭT��apΪ����ָ��
    }
}
 
/*
�������: ���ؼ��ֵ�˳���tree��ÿ�������ú���Visit()һ��������һ��
*/
void TraverseSearchTree(SearchTree tree, void(*Visit)(BT, int)) {
    if(tree) {// �ǿ���
        if(tree->node[0].ptr) {// �е�0������
            TraverseSearchTree(tree->node[0].ptr, Visit);
        }
        for(int i = 1; i <= tree->keynum; i++ ) {
            Visit(*tree, i);
            if(tree->node[i].ptr) { // �е�i������
                TraverseSearchTree(tree->node[i].ptr, Visit);
            }
        }
    }
}
 
int main() {
    Record r[N] = {{24, "1"}, {45, "2"}, {53, "3"}, {12, "4"},
                   {37, "5"}, {50, "6"}, {61, "7"}, {90, "8"},
                   {100, "9"}, {70, "10"}, {3, "11"}, {30, "12"},
                   {26, "13"}, {85, "14"}, {3, "15"}, {7, "16"}};
    SearchTree tree = NULL;//��ʼ��һ�ÿ���
    Result res;//��Ž��
 
    int i;
    for(i = 0; i < N; i++ ) {
        res = SearchPosition(tree, r[i].key);
        if(!res.tag) {
            InsertPosition(tree, r[i], res.pt, res.i);
        }
    }
 
    printf("���ؼ��ֵ�˳�����B-��:\n");
    TraverseSearchTree(tree, print);
    printf("\n����������Ҽ�¼�Ĺؼ���: ");
    while (scanf("%d", &i)) {
        res = SearchPosition(tree, i);
        if(res.tag) {
            print(*(res.pt), res.i);
        } else {
            printf("û�ҵ�\n");
        }
        puts("");
    }
    DestroySearchTree(tree);
}