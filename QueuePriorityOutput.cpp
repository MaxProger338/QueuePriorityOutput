#include<iostream>
#include <initializer_list>
using namespace std;

typedef unsigned int uint;

template<typename T>
class QueuePriorityOutput
{
	private:
        class Node
        {
            public:
                T value;
                int priority;

                Node() : Node(0, 0) {}
                Node(T value, int priority) : value{ value }, priority{ priority } {}

                Node& operator=(const Node& src)
                {
                    if (this == &src)
                        return *this;

                    this->value    = src.value;
                    this->priority = src.priority;

                    return *this;
                }

                friend bool operator>(const Node& node1, const Node& node2)
                {
                    return node1.priority > node2.priority;
                }

                friend bool operator<(const Node& node1, const Node& node2)
                {
                    return node1.priority < node2.priority;
                }

                friend bool operator==(const Node& node1, const Node& node2)
                {
                    return node1.priority == node2.priority;
                }

                friend bool operator!=(const Node& node1, const Node& node2)
                {
                    return node1.priority != node2.priority;
                }
        };

        Node* _arr;
        Node* _top;
        Node* _end;

	public:
        explicit QueuePriorityOutput(uint size)
        {
            _arr = new Node[size];
            _top = _arr - 1;
            _end = _arr + size - 1;
        }

        QueuePriorityOutput(uint size, initializer_list<Node> list) : QueuePriorityOutput(size)
        {
            if (list.size() > size)
                throw "Out of range";

            for (auto i : list)
            {
                enqueue(i.value, i.priority);
            }
        }

        QueuePriorityOutput(const QueuePriorityOutput& queue)
        {
            _arr = new Node[queue.getQueueSize()];
            for (size_t i = 0; i < queue.getQueueSize(); i++)
            {
                _arr[i] = queue._arr[i];
            }

            _end = _arr + queue.getQueueSize() - 1;
            _top = _end - (queue.getQueueSize() - queue.getCurrentSize());
        }

        QueuePriorityOutput(QueuePriorityOutput&& queue)
        {
            _arr = queue._arr;
            _top = queue._top;
            _end = queue._end;

            queue._arr = nullptr;
            queue._top = nullptr;
            queue._end = nullptr;
        }

        ~QueuePriorityOutput()
        {
            delete _arr;
            _arr = nullptr;
            _top = nullptr;
            _end = nullptr;
        }

        bool isFull() const
        {
            return _top == _end;
        }

        bool isEmpty() const
        {
            return _top + 1 == _arr;
        }

        uint getCurrentSize() const
        {
            return _top - _arr + 1;
        }

        uint getQueueSize() const
        {
            return _end - _arr + 1;
        }

        QueuePriorityOutput& clear()
        {
            _top = _arr - 1;
            return *this;
        }

        bool enqueue(T value, int priority)
        {
            if (isFull())
                return false;

            Node node(value, priority);
            
            *++_top = node;

            return true;
        }

        T dequeue()
        {
            if (isEmpty())
                throw "Out of range";

            Node* position = _arr;
            Node* current  = _arr;
            while (current <= _top)
            {
                if (*position < *current)
                {
                    position = current;
                }
                current++;
            }

            T res   = position->value;

            current = _arr;
            while (current < _top)
            {
                if (current >= position)
                {
                    *current = *(current + 1);
                }
                current++;
            }
            _top--;

            return res;
        }

        T peek() const
        {
            if (isEmpty())
                throw "Out of range";

            Node* position = _arr;
            Node* current  = _arr;
            while (current <= _top)
            {
                if (*position < *current)
                {
                    position = current;
                }
                current++;
            }

            return position->value;
        }

        friend ostream& operator<<(ostream& out, const QueuePriorityOutput& queue)
        {
            Node* current = queue._arr;
            while (current <= queue._top)
            {
                out << current->value << '(' << current->priority << ')' << '\t';
                current++;
            }
            out << endl;

            return out;
        }
};

int main()
{
    QueuePriorityOutput<int> q(3);
    q.enqueue(4, 1);
    q.enqueue(14, 2);
    q.enqueue(17, 3);

    cout << q.peek() << endl;

    cout << q << endl;

	return 0;
}
