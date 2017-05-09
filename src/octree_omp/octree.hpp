#ifndef OCTREE_H
#define OCTREE_H

#include "vect.h"
#include "gravity.hpp"

struct OctreeNode
{
	static Body *auxbody;
	struct OctreeNode *ch[8];
	Vect sl, sr;
	float m;
	Vect cm, cv; //cv = average velocity
	Body *body;
	int bodyn;
	OctreeNode()
	{
		int i;
		for (i = 0; i < 8; i++)
			ch[i] = 0;
		m = 0.0;
		body = 0;
		bodyn = 0;
	}
	float size()
	{
		return (sr - sl).abs();
	}
	Body toBody()
	{
		return Body(cm, cv, m);
	}
	void output()
	{
		printf("m = %.2lf, cm = (%.2lg,%.2lg,%.2lg), cv = (%.2lg,%.2lg,%.2lg)\n", m, cm.x, cm.y, cm.z, cv.x, cv.y, cv.z);
	}
	void gather()
	{
		int i;
		m = 0.0;
		for (i = 0; i < 8; i++)
			m += ch[i]->m;
		cm = Vect(0);
		for (i = 0; i < 8; i++)
			cm += (ch[i]->cm * ch[i]->m) / m;
		cv = Vect(0);
		for (i = 0; i < 8; i++)
			cv += (ch[i]->cv * ch[i]->m) / m;
		if (isnan(cv.x) || isnan(cm.x))
		{
			printf("gather ");
			output();
			printf("from: ");
			for (i = 0; i < 8; i++)
				ch[i]->output();
		}
	}
	void expandChildren()
	{
		unsigned i;
		
		for (i = 0; i < 8; i++)
		{
			// find the child's real sl and sr
			Body* body_ch = ch[i]->body;
			int bodyn_ch = ch[i]->bodyn;
			Vect sl_ch = body_ch[0].c;
			Vect sr_ch = body_ch[0].c;
			
			for (int j = 1; j < bodyn_ch; j++)
			{
				sl_ch.x = body_ch[j].c.x < sl_ch.x ? body_ch[j].c.x : sl_ch.x;
				sl_ch.y = body_ch[j].c.y < sl_ch.y ? body_ch[j].c.y : sl_ch.y;
				sl_ch.z = body_ch[j].c.z < sl_ch.z ? body_ch[j].c.z : sl_ch.z;
				sr_ch.x = body_ch[j].c.x > sr_ch.x ? body_ch[j].c.x : sr_ch.x;
				sr_ch.y = body_ch[j].c.y > sr_ch.y ? body_ch[j].c.y : sr_ch.y;
				sr_ch.z = body_ch[j].c.z > sr_ch.z ? body_ch[j].c.z : sr_ch.z;
			}
			ch[i]->sl = sl_ch;
			ch[i]->sr = sr_ch;
			//printf("parent (%.2lf,%.2lf,%.2lf) (%.2lf,%.2lf,%.2lf)\n", sl.x, sl.y, sl.z, sr.x, sr.y, sr.z);
			//printf("chile%d (%.2lf,%.2lf,%.2lf) (%.2lf,%.2lf,%.2lf)\n", i, ch[i]->sl.x, ch[i]->sl.y, ch[i]->sl.z, ch[i]->sr.x, ch[i]->sr.y, ch[i]->sr.z);
		}
	}
	unsigned inChild(const Body &b)
	{
		Vect sm = (sl + sr) / 2;
		return
			(b.c.x < sm.x ? 0u : 1u) |
			(b.c.y < sm.y ? 0u : 2u) |
			(b.c.z < sm.z ? 0u : 4u);
	}
	void distributeBodies()
	{
		int i;
		int cnt[9] = {0};

		for (i = 0; i < 8; i++)
			ch[i] = new OctreeNode;

		for (i = 0; i < bodyn; i++)
			cnt[inChild(body[i])]++;
		for (i = 1; i < 8; i++)
			cnt[i] += cnt[i - 1];
		for (i = bodyn - 1; i >= 0; i--)
			auxbody[--cnt[inChild(body[i])]] = body[i];
		memcpy(body, auxbody, sizeof(body[0]) * bodyn);

		cnt[8] = bodyn;
		for (i = 0; i < 8; i++)
		{
			ch[i]->body = body + cnt[i];
			ch[i]->bodyn = cnt[i + 1] - cnt[i];
		}

		expandChildren();
	}
};

class Octree
{
	OctreeNode *head;
	float theta; //threshold
public:
	void build(Body *body, int n, float _theta)
	{
		theta = _theta;
		if (n <= 0)
		{
			//printf("Octree::build: n = %d\n", n);
			return;
		}

		int i;
		head = new OctreeNode;
		head->body = new Body[n];
		head->bodyn = n;
		memcpy(head->body, body, sizeof(body[0]) * n);
		{
			Vect sl, sr;
			sl = body[0].c - Vect(1, 1, 1);
			sr = body[0].c + Vect(1, 1, 1);
			for (i = 1; i < n; i++)
			{
				sl.x = body[i].c.x < sl.x ? body[i].c.x : sl.x;
				sl.y = body[i].c.y < sl.y ? body[i].c.y : sl.y;
				sl.z = body[i].c.z < sl.z ? body[i].c.z : sl.z;
				sr.x = body[i].c.x > sr.x ? body[i].c.x : sr.x;
				sr.y = body[i].c.y > sr.y ? body[i].c.y : sr.y;
				sr.z = body[i].c.z > sr.z ? body[i].c.z : sr.z;
			}
			head->sl = sl;
			head->sr = sr;
			if (isnan(head->sl.x))
			{
				printf("First NAN!\n");
				throw 1;
			}
		}
		recursiveBuild(head);
		printf("Octree built\n");
	}
	void destroy()
	{
		delete [] head->body;
		if (head)
			recursiveDestroy(head);
		head = 0;
		printf("Octree destroyed\n");
	}
	void destroyAux()
	{
		delete [] head->auxbody;
	}
	Force query(const Body &body)
	{
		return recursiveQuery(head, body);
	}
private:
	void recursiveBuild(OctreeNode *r) //sl, sr of r must be pre-determined
	{
		if (!r)
			return;
		if (r->bodyn == 0)
		{
			r->cm = 0;
			r->m = 0;
			r->cv = 0;
			return;
		}
		//printf("build (%.2lf,%.2lf,%.2lf) (%.2lf,%.2lf,%.2lf) body: %d\n", r->sl.x, r->sl.y, r->sl.z, r->sr.x, r->sr.y, r->sr.z, n);
		if (r->bodyn == 1)
		{
			r->cm = r->body[0].c;
			r->m = r->body[0].m;
			r->cv = r->body[0].v;
			return;
		}
		if (isnan(r->sl.x))
			printf("NAN!\n");
		r->distributeBodies();

		int i;
		for (i = 0; i < 8; i++)
			recursiveBuild(r->ch[i]);

		r->gather();
	}
	void recursiveDestroy(OctreeNode *r)
	{
		int i;
		//printf("destroy %X (%.2lf,%.2lf,%.2lf) (%.2lf,%.2lf,%.2lf)\n", (unsigned)r, r->sl.x, r->sl.y, r->sl.z, r->sr.x, r->sr.y, r->sr.z);
		for (i = 0; i < 8; i++)
			if (r->ch[i])
				recursiveDestroy(r->ch[i]);
		delete r;
	}
	Force recursiveQuery(OctreeNode *r, const Body &body)
	{
		if (!r || r->m == 0.0)
			return Force(0, 0, false);
		bool col = body.size + r->size() > (body.c - r->cm).abs(); //collision test
		bool far = r->size() / (body.c - r->cm).abs() < theta; //far enough
		bool sing = r->ch[0] == 0; //single body
		//col = false;
		if (sing && col)
		{
			float dist;
			if ((body.c - r->body[0].c).abs() != 0.0)
			{
				dist = (body.c - r->body[0].c).abs() - (body.size + r->body[0].size);
				//printf("collision test between %lX,%lX dist=%lf-%lf=%lf\n", (unsigned long)&body, (unsigned long)&r->body[i], (body.c - r->body[i].c).abs(), (body.size + r->body[i].size), dist);
				if (dist < 0)
				{
					Vect vat, vbt;
					collide(body, r->body[0], vat, vbt);
					//assuming no collisions happen simultaneously
					//printf("collision test success\n");
					return Force((body.c - r->body[0].c).unit() * -dist - body.v * dt, vat - body.v, true);
				}
			}
		}
		if (sing || far)
			return calForce2(body, r->toBody());
		int i;
		Force f = Force(0, 0, false);
		for (i = 0; i < 8; i++)
			f += recursiveQuery(r->ch[i], body);
		return f;
	}
};

#endif