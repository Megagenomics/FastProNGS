#ifndef _QC_H_
  #include "qc.h"
#endif

struct _Entry{
	int cost;
	int matches;
	int origin;
};

struct _Match{
	int origin;
	int cost;
	int matches;
	int ref_stop;
	int query_stop;
};

int locate(
  char *reference,
  char *query,
  float max_error_rate,
  int flags,
  int _min_overlap,
  int *res)
{
int m = strlen(reference);
int n = strlen(query);
struct _Entry *column=(struct _Entry *)xmalloc(sizeof( struct _Entry)*(m+1));
int start_in_ref = flags & START_WITHIN_SEQ1;
int start_in_query = flags & START_WITHIN_SEQ2;
int stop_in_ref = flags & STOP_WITHIN_SEQ1;
int stop_in_query = flags & STOP_WITHIN_SEQ2;

int _insertion_cost = 1;
int _deletion_cost = 1;

int i,j;
int k=(int)(max_error_rate*m);

int max_n = n;
int min_n = 0;
if(!start_in_query)
  max_n = n<(m + k)? n : (m+k);
if(!stop_in_query)
  min_n = 0>(n - m - k)?0:(n - m - k);

if(!start_in_ref && !start_in_query){
  for(i=0;i<(m + 1);i++){
	column[i].matches = 0;
	column[i].cost = (i>min_n?i:min_n) * _insertion_cost;
	column[i].origin = 0;
  }
}else if(start_in_ref && !start_in_query){
  for(i=0;i<(m + 1);i++){
	column[i].matches = 0;
	column[i].cost = min_n * _insertion_cost;
	column[i].origin = 0<(min_n - i)?0:(min_n - i);
  }
}else if(!start_in_ref && start_in_query){
  for(i=0;i<(m + 1);i++){
	column[i].matches = 0;
	column[i].cost = i * _insertion_cost;
	column[i].origin = 0> min_n - i?0:min_n - i;
  }
}else{
  for(i=0;i<(m + 1);i++){
	column[i].matches = 0;
	column[i].cost = (i<min_n?i:min_n) * _insertion_cost;
	column[i].origin = min_n - i;
  }
}

struct _Match best;
best.ref_stop = m;
best.query_stop = n;
best.cost = m + n;
best.origin = 0;
best.matches = 0;

int last = m<(k + 1)?m:(k+1);
if(start_in_ref)
    last = m;

int cost_diag;
int cost_deletion;
int cost_insertion;
int origin, cost, matches;
int length;
int characters_equal;
struct _Entry tmp_entry;

for(j=min_n + 1;j<(max_n + 1);j++){
  tmp_entry = column[0];

  if(start_in_query)
	column[0].origin = j;
  else
	column[0].cost = j * _insertion_cost;
  for(i=1;i<last + 1;i++){
	characters_equal = reference[i-1] == query[j-1]?1:0;
	if(characters_equal){
	  cost = tmp_entry.cost;
	  origin = tmp_entry.origin;
		matches = tmp_entry.matches + 1;
    }else{
	  cost_diag = tmp_entry.cost + 1;
	  cost_deletion = column[i].cost + _deletion_cost;
	  cost_insertion = column[i-1].cost + _insertion_cost;
	  if(cost_diag <= cost_deletion && cost_diag <= cost_insertion){
	    /* MISMATCH */
		cost = cost_diag;
		origin = tmp_entry.origin;
		matches = tmp_entry.matches;
      }else if(cost_insertion <= cost_deletion){
	    /* INSERTION */
		cost = cost_insertion;
		origin = column[i-1].origin;
		matches = column[i-1].matches;
      }else{
	    /* DELETION */
		cost = cost_deletion;
		origin = column[i].origin;
		matches = column[i].matches;
      }
    }
	/* remember current cell for next iteration*/
	tmp_entry = column[i];
	column[i].cost = cost;
	column[i].origin = origin;
	column[i].matches = matches;
  }
  while(last >= 0 && column[last].cost > k)
	  last -= 1;
	/* last can be -1 here, but will be incremented next.*/
	/* TODO if last is -1, can we stop searching?*/
	if(last < m){
	  last += 1;
  }else if(stop_in_query){
	  /* Found a match. If requested, find best match in last row.*/
	  /* length of the aligned part of the reference*/
	  length = m + (column[m].origin<0?column[m].origin:0);
	  cost = column[m].cost;
	  matches = column[m].matches;
	  if(length >= _min_overlap && cost <= length * max_error_rate && (matches > best.matches || (matches == best.matches && cost < best.cost))){
		/*update*/
		best.matches = matches;
		best.cost = cost;
		best.origin = column[m].origin;
		best.ref_stop = m;
		best.query_stop = j;
		if(cost == 0 && matches == m){
	    /* exact match, stop early */
		  break;
    }
    }
  }
}
/* column finished */
if(max_n == n){
  int first_i = stop_in_ref ? 0 : m;
  /* search in last column # TODO last?*/
  for(i=first_i;i<(m+1);i++){
    length = i + (column[i].origin<0?column[i].origin:0);
	cost = column[i].cost;
	matches = column[i].matches;
	if(length >= _min_overlap && cost <= length * max_error_rate && (matches > best.matches || (matches == best.matches && cost < best.cost))){
	  /* update best */
    best.matches = matches;
	  best.cost = cost;
	  best.origin = column[i].origin;
	  best.ref_stop = i;
	  best.query_stop = n;
    }
  }
}
if(best.cost == (m + n)){
	FREE(column);
  return 0;
}
int start1=0, start2=0;
if(best.origin >= 0){
  start1 = 0;
  start2 = best.origin;
}else{
  start1 = -best.origin;
  start2 = 0;
}
FREE(column);
res[0]=start1,res[1]=best.ref_stop,res[2]=start2,res[3]=best.query_stop,res[4]=best.matches,res[5]=best.cost;
return 1;
}

void compare_prefixes(
    char *reference,
    char *query,
    int *res)
{
int m = strlen(reference);
int n = strlen(query);
int length = m<n?m:n;
int i, matches = 0;

for(i=0;i<length;i++){
	if(reference[i] == query[i])
	matches ++;
}
res[0]=0,res[1]=length,res[2]=0,res[3]=length,res[4]=matches,res[5]=length - matches;
}

void compare_suffixes(
    char *reference,
    char *query,
    int *res)
{
int m = strlen(reference);
int n = strlen(query);
int length = m<n?m:n;
int i, matches = 0;

for(i=length;i>=0;i--){
	if(reference[i] == query[i])
	matches ++;
}
res[0]=m-length,res[1]=length,res[2]=n-length,res[3]=length,res[4]=matches,res[5]=length - matches;
}